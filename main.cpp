/**
 * Author: Jason B. Shew - 251285678
 * Date: 2023-10-04
 * Description: This is the main class of the program, required by the assignment requirements.
 * */
#include <Wt/WApplication.h>
#include "WordGame.h" // WordGame class is declared in WordGame.h

/**
 * This is the main() function of the main class.
 * */
int main(int argc, char **argv)
{
    /*
     * Your main method may set up some shared resources, but should then
     * start the server application (FastCGI or httpd) that starts listening
     * for requests, and handles all of the application life cycles.
     *
     * The last argument to WRun specifies the function that will instantiate
     * new application objects. That function is executed when a new user surfs
     * to the Wt application, and after the library has negotiated browser
     * support. The function should return a newly instantiated application
     * object.
     */
    return Wt::WRun(argc, argv, [](const Wt::WEnvironment &env) {
        /*
         * You could read information from the environment to decide whether
         * the user has permission to start a new application
         */
        return std::make_unique<WordGame>(env);
    });
}

