#ifndef WORDGAME_H
#define WORDGAME_H

/**
 * Author: Jason B. Shew - 251285678
 * Date: 2023-10-04
 * Description: This is the header file for WordGame class.
 **/

#pragma once
#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <utility>
#define GAME_NAME "WordGame V1.0"
#define WORDLIST_FILE "words.txt"
#define MAX_GUESS 6
#define WORD_LENGTH 5

class WordGame : public Wt::WApplication { // WordGame class

public:
    explicit WordGame(const Wt::WEnvironment& env); // WordGame class constructor
    ~WordGame() override; // WordGame class destructor

private:
    int userCurrentGuess = 0; // a counter of user's guesses
    Wt::WLineEdit *userInput_; // user input area
    Wt::WText *welcome_, *infoDisplay_, *chancesLeft_, *tileGrid[WORD_LENGTH * MAX_GUESS], *author_;
    // information area, displaying welcome information, interactive information, chances left, word tiles, and author information
    Wt::WPushButton *submitButton; // the button user uses to submit a guess
    std::vector<std::string> guessedWords; // an array of user's guessed words
    std::string mysteryWord; // the word to guess
    std::string wordListFile = WORDLIST_FILE;  // the dictionary file that contains all possible words
    std::vector<std::string> wordList = readWordsFromFile(wordListFile); // the array of all possible words

    // function prototypes
    bool isAllLetters(std::string basicString);

    bool isValidWord(std::string userWord);

    int createTileRow(int sequence, std::string userGuessedWord);

    bool hasBeenGuessed(std::string word);

    std::vector<std::string> readWordsFromFile(const std::string &filePath);

    std::string getRandomWord();

    void paintTileRow(int sequence, std::string userWord);

    bool isSuccessful(const std::string& userWord);

    void insertBlankLines(int amount);
};

#endif // WORDGAME_H

