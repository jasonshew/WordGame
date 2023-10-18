/**
 * Author: Jason B. Shew - 251285678
 * Date: 2023-10-04
 * Description: This is the WordGame class. Run with argument: --docroot . --http-address 0.0.0.0 --http-port 16456
 * Read README.txt to learn how to compile and run in Terminal.
 * Check the demo picture to learn how to run this program in your web browser (Google Chrome recommended).
 * */



#include "WordGame.h"



/**
 * This is the constructor of the WordGame class
 */
WordGame::WordGame(const Wt::WEnvironment& env)
        : Wt::WApplication(env) {

    setTitle(GAME_NAME); // set title of the webpage
    mysteryWord = getRandomWord(); // get a random word from the dictionary

    std::cout << "[DEV INFO] THE WORD IS: " + mysteryWord << std::endl; // Comment this line out for more fun :-)
    // For developer's convenience, the console prints out the mystery word to guess.
    // Keep only one browser tab open, so the console will not get refreshed frequently.

    // Add the following widgets to the container
    // Welcome information with decoration styles
    welcome_ = root()->addWidget(std::make_unique<Wt::WText>(GAME_NAME));
    welcome_ -> decorationStyle().font().setFamily(Wt::FontFamily::SansSerif);
    welcome_ ->decorationStyle().font().setSize(40);
    welcome_ ->decorationStyle().font().setWeight(Wt::FontWeight(900)); // Add title of the page with paddings
    welcome_ ->decorationStyle().setForegroundColor(Wt::WColor(164,164,164));
    insertBlankLines(2);

    for (int k = 0; k < MAX_GUESS * WORD_LENGTH; k++) { // place empty tiles for displaying user's guesses
        tileGrid[k] = root()->addWidget(std::make_unique<Wt::WText>(""));
        tileGrid[k] ->decorationStyle().font().setSize(50); // set the size of each tile
        if (k % WORD_LENGTH == WORD_LENGTH - 1) // at the end of every word (the last title of each row)
            insertBlankLines(1); // there needs to be a line break
    }

    insertBlankLines(2);

    // Show prompt, user input area, and a submit button
    root()->addWidget(std::make_unique<Wt::WText>("Enter your guess (" + std::to_string(WORD_LENGTH) +  " letters): "));
    userInput_ = root()->addWidget(std::make_unique<Wt::WLineEdit>());
    userInput_->setFocus();
    submitButton = root()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));

    insertBlankLines(1);

    // an area to display interactive information
    infoDisplay_ = root()->addWidget(std::make_unique<Wt::WText>());
    insertBlankLines(1);

    // display number of chances left
    chancesLeft_ = root()->addWidget(std::make_unique<Wt::WText>());
    chancesLeft_->decorationStyle().setForegroundColor(Wt::WColor(8, 114, 181)); // blue;
    chancesLeft_->setText("Chances left: " + std::to_string(MAX_GUESS));

    insertBlankLines(1);

    // display author's information
    author_ = root()->addWidget(std::make_unique<Wt::WText>());
    author_->setText("Author: Jason B. Shew (jshew3@uwo.ca | 251285678)");

   // action performed when the submit button is pressed
    auto submit = [this] {
        while (userCurrentGuess < MAX_GUESS) { // loops over the guesses

            std::string userWord = userInput_->text().toUTF8(); // get user input string
            userWord.erase(std::remove_if(userWord.begin(), userWord.end(), ::isspace), userWord.end()); // removes spaces from user input
            std::transform(userWord.begin(), userWord.end(), userWord.begin(), ::toupper); // turn user input into uppercase

            if (userWord.length() == 0) { // if user input is empty
                infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                infoDisplay_->setText("You didn't enter anything."); // returns a warning
                break;
            }

            else if (userWord.length() == 5 && isAllLetters(userWord)) { // if user input is a five-letter word

                infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(9, 121, 105)); // green
                std::string result("The word you guessed is: ");

                if (createTileRow(guessedWords.size(), userWord) == 1) { // if the word is from the dictionary and can make a tile row

                    if (hasBeenGuessed(userWord)) { // if the word has been guessed
                        for (int j = 0; j < WORD_LENGTH; j++) {
                            tileGrid[userCurrentGuess * WORD_LENGTH + j]->setText(""); // clears the tile row for this guess because the word has already been displayed above
                            tileGrid[userCurrentGuess * WORD_LENGTH + j]->setPadding(0);
                        }
                        infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                        infoDisplay_->setText("You've guessed that word. Try a new one.");
                        break;
                    }

                    userCurrentGuess++; // otherwise counts as a successful guess
                    infoDisplay_->setText(result + userWord); // confirm user guess
                    bool state = isSuccessful(userWord); // get the game state; if user succeeds then game restarts from here
                    chancesLeft_->setText("Chances left: " + std::to_string(MAX_GUESS - userCurrentGuess)); // chances left is one more less
                    guessedWords.push_back(userWord); // push the word to guessed words list

                    if (!state && userCurrentGuess == MAX_GUESS) { // if the user runs out of chances without success
                        infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(103, 0, 211)); // purple text;
                        infoDisplay_->decorationStyle().setBackgroundColor(Wt::WColor(255, 255, 0)); // yellow background;
                        submitButton->disable(); // disable submit button
                        infoDisplay_->setText("You've run out of chances. The correct word is: " + mysteryWord);
                        // informs the user of the failure and the correct word
                        insertBlankLines(1);
                        Wt::WText* restart = root()->addWidget(std::make_unique<Wt::WText>("The game will restart automatically... if JavaScript is enabled for your browser..."));
                        // restarts the game in 5 seconds
                        restart->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                        restart->doJavaScript("setTimeout(function(){ location.reload(); }, 5000);");
                    }

                    break;
                }

                else { // if the user enters a five-letter word which doesn't exist in the dictionary
                    infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                    infoDisplay_->setText(userWord + " does not exist in the dictionary.");
                    break;
                }
            }

            else if (userWord.length() == 5 && !isAllLetters(userWord)) { // if the user enters non-letter characters
                infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                infoDisplay_->setText("Only English letters are accepted. Try again.");
                break;
            }

            else if (userWord.length() != 5 && isAllLetters(userWord)) { // if the user enters more or less than five letters
                infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                infoDisplay_->setText("That was not " + std::to_string(WORD_LENGTH) + " letters. Try again.");
                break;
            }

            else { // other invalid inputs
                infoDisplay_->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
                infoDisplay_->setText("Invalid input. Only " + std::to_string(WORD_LENGTH) + "-letter English words are accepted. Try again.");
                break;
            }
        }
    };

    submitButton->clicked().connect(submit); // connect submit function with the button

}



/**
 * @name isAllLetters
 * @description This function checks if user input contains only letters
 * @param std::string str: any string
 * @return true if the string contains only English letters, false otherwise
 * */
bool WordGame::isAllLetters(std::string str) {
    for (char c : str) {
        if (!std::isalpha(static_cast<unsigned char>(c))) {
            return false;  // at least a character is not a letter
        }
    }
    return true;  // All characters are letters
}



/**
 * @name isValidWord
 * @description This function checks if a user's guess exists in the dictionary.
 * @param std::string userWord: the user-input word to check validity for
 * @return: true if the word exists in the dictionary, false otherwise
 * */
bool WordGame::isValidWord(std::string userWord) {
    std::transform(userWord.begin(), userWord.end(), userWord.begin(), ::toupper);
    for (const auto & word : wordList) {
        if (word == userWord) {
            return true;
        }
    }
    return false;
}



/**
 * @name createTileRow
 * @description This function creates a row of tiles for a valid user-guessed word
 * @param int sequence: the user's n-th guess, starting from 0
 * @param std::string userGuessedWord: the userGuessedWord that the user has just guessed
 * @return: integer 1 if the user's guess is a valid English userGuessedWord and gets displayed on the screen, 0 otherwise
 * */

int WordGame::createTileRow(int sequence, std::string userGuessedWord) { // sequence means the user's n-th guess, starting from 0
    std::transform(userGuessedWord.begin(), userGuessedWord.end(), userGuessedWord.begin(), ::toupper); // turns userGuessedWord into uppercase
    if (isValidWord(userGuessedWord)) { // if the userGuessedWord is a valid word from the dictionary
        int start = sequence  * WORD_LENGTH; // starting index of the tile (the first letter of the n-th guess)
        int end = (sequence + 1) * WORD_LENGTH; // ending index of the tile (the last letter of the n-th guess)
        while (start < end) {
            std::string output = "";
            output.push_back(userGuessedWord[start % WORD_LENGTH]); // concatenate the char array to an empty string to get the userGuessedWord string
            tileGrid[start]->setText(output); // push each letter onto the corresponding tile
            tileGrid[start]->decorationStyle().font().setFamily(Wt::FontFamily::Monospace); // change font
            start++; // process the next letter of the userGuessedWord
        }
        paintTileRow(sequence, userGuessedWord);
        return 1;
    }
    return 0;
}



/**
 * @name hasBeenGuessed
 * @description This function checks if a word has been guessed. Guessed words do not count toward chances left.
 * @param std::string word: the current guessed word
 * @return true if the word has been guessed by the user, false otherwise
 * */

bool WordGame::hasBeenGuessed(std::string word) {
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    auto iter = std::find(guessedWords.begin(), guessedWords.end(), word);
    if (iter != guessedWords.end()) { // if the iterator doesn't yet reach the end of the word list then it's a guessed word
        return true;
    } else {
        return false;
    }
}



/**
 * @name readWordsFromFile
 * @description This function helps read data from a file.
 * @param std::string filePath: the path and name of the file to read
 * @return an array of words ripped from the file, words converted to all caps
 * */
std::vector<std::string> WordGame::readWordsFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> words; // an empty array of words
    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::toupper); // turn words into all caps
            words.push_back(word); // push each word from the file into the array
        }
        file.close();
    } else {
        std::cerr << "Error opening file: " << filePath << std::endl;
    }

    return words; // returns a word list from the file
}



/**
 * @name getRandomWord
 * @description This function gets a random word from the dictionary
 * @param none
 * @return a random word generated by this function
 * */
std::string WordGame::getRandomWord() {
    // Read words from the file
    std::string theRandomWord;

    // Get a random word from the list

    if (wordList.empty()) {
        return "";  // Return an empty string if there are no words
    }

    // Providing a seed value
    srand((unsigned) time(nullptr));

    // Get a random number
    int random = rand();

    // Generate a random index
    std::size_t randomIndex = random % wordList.size();
    // Return the randomly selected word
    theRandomWord = wordList[randomIndex];
    return theRandomWord;
}



/**
 * @name paintTileRow
 * @description This function paints a row of tiles according to the status of each letter
 * @param int sequence: the user's n-th guess, n starting from 0
 * @param std::string userWord: the word that the user has just guessed
 * @return none
 * */
void WordGame::paintTileRow(int sequence, std::string userWord) {

    std::string userWordCopy = std::move(userWord); // make a copy of userWord, so that the original string is not affected
    std::string mysteryWordCopy = mysteryWord; // make a copy of mysteryWord, so that the original string is not affected

    for (int i = 0; i < WORD_LENGTH; i++) { // initialize the size of each tile and paint each tile using "default tile" colour scheme
        tileGrid[sequence * WORD_LENGTH + i] ->setPadding(14); // set paddings
        tileGrid[sequence * WORD_LENGTH + i] ->setMargin(9); // set margins between the tiles
        tileGrid[sequence * WORD_LENGTH + i]->decorationStyle().setForegroundColor(
                Wt::WColor(255, 255, 255)); // letters are always in white
        tileGrid[sequence * WORD_LENGTH + i]->decorationStyle().setBackgroundColor(Wt::WColor(100, 100, 100)); // dark grey background
    }

    for (int i = 0; i < WORD_LENGTH; i++) { // try to detect correct letters in correct positions
        if (userWordCopy[i] == mysteryWordCopy[i]) {
            userWordCopy.replace(i, 1, "-"); // replace correct letters with different symbols, so they won't get matched again in the same word
            mysteryWordCopy.replace(i, 1, "_");
            {
                tileGrid[sequence * WORD_LENGTH + i]->decorationStyle().setBackgroundColor( // paint correct letters using "correct tile" colour scheme
                        Wt::WColor(8, 180, 37)); // green background
            }
        }
    }
    for (int i = 0; i < WORD_LENGTH; i++) { // try to detect correct letters in wrong positions
        for (int j = 0; j < WORD_LENGTH; j++) {
            if ((i != j) && (userWordCopy[i] == mysteryWordCopy[j])) {
                userWordCopy.replace(i, 1, "."); // replace correct letters with different symbols, so they won't get matched again in the same word
                mysteryWordCopy.replace(j, 1, "*");
                tileGrid[sequence * WORD_LENGTH + i]->decorationStyle().setBackgroundColor( // paint correct letters using "misplaced tile" colour scheme
                        Wt::WColor(255, 192, 0)); // golden yellow background
            }
        }
    }

}



/**
 * @name isSuccessful
 * @description This function checks if the user succeeds with the current guess. If so, relevant information will be displayed and game will restart automatically (with JavaScript enabled)
 * @param std::string userWord: the user's current guess
 * @return true if the user succeeds, false otherwise
 * */
bool WordGame::isSuccessful(const std::string& userWord){
    if (userWord == mysteryWord) { // if the word is just the mystery word
        chancesLeft_->hide(); // hide chances left
        submitButton->disable(); // disable submit button
        infoDisplay_->setText("Amazing! You made it!"); // then informs the user of success
        insertBlankLines(1);
        // restarts the game using JavaScript
        Wt::WText* restart = root()->addWidget(std::make_unique<Wt::WText>("The game will restart automatically... if JavaScript is enabled for your browser..."));
        // Add JavaScript to restart the page after 6 seconds
        restart->decorationStyle().setForegroundColor(Wt::WColor(196, 30, 58)); // red;
        restart->doJavaScript("setTimeout(function(){ location.reload(); }, 6000);");
        return true;
    }
    return false;
}



/**
 * @name insertBlankLines
 * @description This function adds certain number of blank lines to the container
 * @param int number: the number of blank lines desired; 0 if only a line break is wanted
 * @return none
 * */
void WordGame::insertBlankLines(int number) {
    for (int i = 0; i < number + 1; i++) {
        root()->addWidget(std::make_unique<Wt::WBreak>());
    }
}

WordGame::~WordGame() = default;  // destructor

