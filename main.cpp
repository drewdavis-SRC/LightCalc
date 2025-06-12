#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <limits>
#include <windows.h>

using namespace std;

void ResetTerminal()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(hStdOut, &csbi);
    FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hStdOut, coord);
}

struct Tap 
{
    // values needed for each tap
    int port_count;
    int tap_value_db;
    double max_insertion_loss;
    double max_drop_loss;
    bool is_terminating;
};

vector<Tap> populateTaps() 
{
    // all possible taps
    vector<Tap> taps = {
        // 2-port taps
        {2, 21, 0.40, 21.95, false},
        {2, 19, 0.50, 18.45, false},
        {2, 17, 0.60, 17.55, false},
        {2, 15, 0.80, 15.25, false},
        {2, 14, 1.00, 13.25, false},
        {2, 12, 1.30, 11.85, false},
        {2, 10, 2.00, 10.05, false},
        {2, 8, 2.70, 8.75, false},
        {2, 7, 4.10, 7.25, false},
        {2, 5, 6.00, 6.05, false},
        {2, 4, 0.00, 3.95, true},  // Terminating
        // 4-port taps
        {4, 21, 0.60, 20.95, false},
        {4, 19, 0.80, 18.65, false},
        {4, 17, 1.00, 16.65, false},
        {4, 15, 1.30, 15.25, false},
        {4, 13, 2.00, 13.45, false},
        {4, 11, 2.70, 12.15, false},
        {4, 10, 4.10, 10.65, false},
        {4, 9, 6.00, 9.45, false},
        {4, 7, 0.00, 7.35, true},  // Terminating
        // 8-port taps
        {8, 21, 0.80, 21.95, false},
        {8, 19, 1.00, 19.95, false},
        {8, 17, 1.30, 18.55, false},
        {8, 15, 2.00, 16.75, false},
        {8, 14, 2.70, 15.45, false},
        {8, 12, 4.10, 13.95, false},
        {8, 11, 0.00, 12.75, true}   // Terminating
    };
    return taps;
}

void MainLightQuestion()
{
    std::cout << "** NOTE: Use fullscreen for the best performance.\n";
    std::cout << "\nMain line light levels are usually 1-3 dB. This is needed to make proper calculations.";
    std::cout << "\nThe program is able to handle numbers up to 2 decimal places (IE: 2.45).\n";
    std::cout << "\nEnter the main line light level: ";
}

void MenuTitle()
{
    std::cout << "===== FTTH Tap Calculator Menu =====\n"
             << "1. Create New Chain (clears existing)\n"
             << "2. View Current Chain\n"
             << "3. Clear Current Chain\n"
             << "4. Insert Tap into Chain\n"
             << "5. Replace Tap on Chain\n"
             << "6. Calculate Light Loss\n"
             << "7. View Light Loss Table\n"
             << "8. Exit\n"
             << "====================================\n"
             << "\nEnter your choice: ";
}

void ChainCreationTitle()
{
    std::cout << "=======================";
    std::cout << "\nChain Creation Selected";
    std::cout << "\n=======================";
    std::cout << std::endl;
}

void ViewChainTitle()
{
    std::cout << "===================";
    std::cout << "\nChain View Selected";
    std::cout << "\n===================";
    std::cout << std::endl;
}

void ClearChainTitle()
{
    std::cout << "====================";
    std::cout << "\nClear Chain Selected";
    std::cout << "\n====================";
}

void TapInsertionTitle()
{
    std::cout << "======================";
    std::cout << "\nTap Insertion Selected";
    std::cout << "\n======================";

    std::cout << "\n\nThe current chain is shown below.\n";
    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
}

void TapReplacementTitle()
{
    std::cout << "========================";
    std::cout << "\nTap Replacement Selected";
    std::cout << "\n========================";

    std::cout << "\n\nThe current chain is shown below.\n";

    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
}

void LossCalculationTitle()
{
    std::cout << "===============================";
    std::cout << "\nLight Loss Calculaiton Selected";
    std::cout << "\n===============================";
    std::cout << std::endl;

    std::cout << "\nPosition | Ports | Tap Value (dB) | Footage (ft) | Main Light Level (dB) | Drop Light Level (dB)\n";
    std::cout << "--------------------------------------------------------------------------------------------------\n";
}

void LightTableTitle()
{
    std::cout << "=========================";
    std::cout << "\nLight Table View Selected";
    std::cout << "\n=========================";
    std::cout << std::endl;
    
    std::cout << "\nPorts | Tap Value (dB) | Max Insertion Loss (dB) | Max Drop Loss (dB) | Terminating" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------\n";
}

vector<Tap> CreateNewChain(const vector<Tap>& all_taps)
{

    ChainCreationTitle();

    // initialize chain to store user selection
    vector<Tap> chain;

    // initialize and receive number of taps in the chain for iteration
    int num_taps;
    std::cout << "\nEnter number of taps in chain: ";
    std::cin >> num_taps;

    // reset to save space in terminal
    ResetTerminal();

    // call the title again
    ChainCreationTitle();

    // create taps for the amount entered
    for (int i = 0; i < num_taps; i++)
    {
        // tell user which tap they're creating in the chain
        bool is_last = (i == num_taps - 1);
        std::cout << "\n==== Tap " << i + 1 << "/" << num_taps << " ====\n";
        
        // select port count of tap
        int port_count;
        std::cout << "Enter port count (2/4/8): ";
        std::cin >> port_count;
        
        // create new vector and fill it with taps of the selected port count
        vector<Tap> available;
        for (const Tap& t : all_taps) 
        {
            if (t.port_count == port_count)
            {
                available.push_back(t);
            }
        }

        // if the available chian is empty, show in terminal and tell user they picked an invalid port count
        if (available.empty()) 
        {
            // reset terminal and call title to make error message be uniform
            ResetTerminal();
            ChainCreationTitle();

            std::cout << "\nThere are no vaild taps with that port count.\n";

            // decrement i so we go until we correctly fill the current tap
            i--;
            continue;
        }

        // display tap options by iterating through available vector
        std::cout << "\nAvailable taps:\n";
        for (size_t j = 0; j < available.size(); j++) 
        {
            std::cout << j + 1 << ". " << available[j].tap_value_db << " dB (Max Insertion Loss: " 
                 << available[j].max_insertion_loss << " dB)\n";
        }

        // get selection of the tap value
        int choice;
        std::cout << "\nChoose tap: ";
        std::cin >> choice;

        // if the user enters an invalid number
        if (choice < 1 || choice > available.size()) 
        {
            ResetTerminal();
            ChainCreationTitle();
            std::cout << "\nInvalid tap value. Restarting.\n";
            i--;
            continue;
        }

        // fill user chain with the choice
        chain.push_back(available[choice-1]);

        // reset the terminal and call the title again so we dont scroll on the terminal
        ResetTerminal();
        ChainCreationTitle();
    }

    // reset terminal and return the chain the user will be seeing
    ResetTerminal();
    return chain;
}

void CreationClear(vector <Tap>& chain)
{
    // removed empty check since this will only execute if the chain is not empty
    /*
    // make sure we dont clear a chain that hasn't been created
    if (chain.empty())
    {
        std::cout << "Please create a chain first.\n\n";
        return;
    }
    */

    // show how many taps will be cleared
    std::cout << "Clearing previous chain with " << chain.size() << " taps. ";

    // clear taps in the chain
    chain.clear();

    // output the chain is clear
    std::cout << "Chain is now clear.\n\n";
}

void ViewChain(const vector<Tap>& chain, float main_light_level)
{
    // call title for selection
    ViewChainTitle();

    // output columns and divider
    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB) | Terminating\n";
    std::cout << "----------------------------------------------------------------------------------\n";
    
    // iterate through current chain
    for (size_t i = 0; i < chain.size(); i++) 
    {
        const Tap& t = chain[i];

        // output each row of the table with all tap elements and dividers
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << "     | ";
        std::cout << t.tap_value_db;
        std::cout << " dB    | ";
        std::cout << t.max_insertion_loss;
        std::cout << " dB            | ";
        std::cout << t.max_drop_loss;
        std::cout << " dB           | ";
        std::cout << (t.is_terminating ? "Yes" : "No");
        std::cout << std::endl;
    }

    // wait for user to be done
    std::cout << std::endl;
    system("pause");

    // remove everything from terminal for cleanliness
    ResetTerminal();
}

void ClearChain(vector <Tap>& chain)
{
    // call feature's title
    ClearChainTitle();

    // show how many taps will be cleared
    std::cout << "\n\n" << chain.size() << " taps found. ";

    // clear taps in the chain
    chain.clear();

    // output the chain is clear
    std::cout << "Chain is now clear.";

    // simulate processing so user can see something happened
    sleep(2);
    std::cout << "\n\nProceeding back to menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);

    // reset for cleanliness
    ResetTerminal();
}

void InsertTap(vector<Tap>& chain, const vector<Tap>& all_taps)
{
    // call title
    TapInsertionTitle();

    // output rows of taps for the entire current chain
    for (size_t i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // output rows of tap elements and dividers
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << "     | ";        
        std::cout << t.tap_value_db;
        std::cout << " dB    | ";
        std::cout << t.max_insertion_loss;
        std::cout << "               | ";
        std::cout << t.max_drop_loss;
        std::cout << std::endl;
    }

    // tell user about how incorrect input has different handling than other features 
    std::cout << "\n** NOTE: Invalid entries will reset to menu.\n";

    // get position from user for insertion
    int position;
    std::cout << "\nInsert position (1-" << chain.size()+1 << "): ";
    std::cin >> position;

    // make sure position cannot be outside the range of the chain
    if (position < 1 || position > chain.size()+1) 
    {
        // wrong input message
        std::cout << "\nInvalid position!";

        //simulate processing so user can see what happened
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }

    // convert to 0-based index for vectors so we are inserting at the correct place 
    position--;

    // make sure we remember if a tap is the end of a chain
    bool inserting_at_end = (position == chain.size());

    // get port count for insertion
    int port_count;
    std::cout << "\nPort count (2/4/8): ";
    std::cin >> port_count;

    // filter available taps
    vector<Tap> available;
    for (const Tap& t : all_taps) 
    {
        if (t.port_count == port_count)
        {
            available.push_back(t);
        }
    }

    // if available is empty, port count was wrong
    if (available.empty()) 
    {
        // wrong input message
        std::cout << "\nNo valid taps!";
        
        //simulate processing so user can see what happened
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }

    // show all choices for insertion based off port count
    std::cout << "\nAvailable taps:\n";
    for (size_t i = 0; i < available.size(); i++) 
    {
        std::cout << i + 1 << ". " << available[i].tap_value_db << " dB\n";
    }

    // get tap value choice
    int choice;
    std::cout << "\nChoose tap: ";
    std::cin >> choice;

    // dont let the choice be out of range
    if (choice < 1 || choice > available.size()) 
    {
        // wrong input message
        std::cout << "\nInvalid!";
        
        //simulate processing so user can see what happened
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }

    // initalize new variable that holds the choice
    Tap new_tap = available[choice-1];

    // update previous last tap if inserting at end
    if (inserting_at_end && !chain.empty()) 
    {
        chain.back().is_terminating = false;
    }

    // insert new tap at desired position
    chain.insert(chain.begin() + position, new_tap);

    // show user insertion was successful
    std::cout << "\nTap inserted successfully!";
    
    // simulate processing so user can see what happened
    sleep(2);
    std::cout << "\n\nProceeding back to menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);

    // reset for cleanliness
    ResetTerminal();
}

void ReplaceTap(vector<Tap>& chain, const vector<Tap>& all_taps)
{
    TapReplacementTitle();

    // iterate through current chain for viewing
    for (size_t i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // show each element of the tap
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << " dB          | ";        
        std::cout << t.tap_value_db;
        std::cout << " dB    | ";
        std::cout << t.max_insertion_loss;
        std::cout << "             | ";
        std::cout << t.max_drop_loss;
        std::cout << std::endl;
    }

    std::cout << "\n** NOTE: Invalid position resets to menu.\n";

    // get the position for replacement
    int position;
    std::cout << "\nEnter the position of the tap to replace: ";
    std::cin >> position;

    // dont let position go out of range
    if (position < 1 || position > chain.size()+1) 
    {
        // wrong input message
        std::cout << "\nInvalid position!";

        // sim prcoessing
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }
    position--; // users see i + 1, move back to 0 based index for vectors for correct replacement

    // get the port count of replacement tap
    int port_count;
    std::cout << "\nEnter port count (2/4/8): ";
    std::cin >> port_count;

    // fill available vector of taps with that port count
    vector<Tap> available;
    for (const Tap& t : all_taps) 
    {
        if (t.port_count == port_count)
        {
            available.push_back(t);
        }
    }

    if (available.empty()) 
    {
        // wrong input msg
        std::cout << "\nNo valid taps! Retry.";
                
        //simulate processing so user can see what happened
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }

    // display available taps
    std::cout << "Available taps:\n";

    for (size_t j = 0; j < available.size(); j++) 
    {
        std::cout << j+1 << ". " << available[j].tap_value_db << " dB (Max Insertion Loss: " 
                << available[j].max_insertion_loss << " dB)\n";
    }

    // get user choice of tap
    int choice;
    std::cout << "\nChoose tap: ";
    std::cin >> choice;

    // dont let it be outside range of vector
    if (choice < 1 || choice > available.size())
    {
        // wrong input msg
        std::cout << "\nInvalid!";
                
        //simulate processing so user can see what happened
        sleep(2);
        std::cout << "\n\nProceeding back to menu.";
        sleep(1);
        std::cout << ".";
        sleep(1);

        // reset for cleanliness
        ResetTerminal();
        return;
    }

    // replace old tap with new
    chain[position] = available[choice - 1];

    // show user insertion was successful
    std::cout << "\nTap replaced successfully!";
    
    // simulate processing so user can see what happened
    sleep(2);
    std::cout << "\n\nProceeding back to menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);

    // reset for cleanliness
    ResetTerminal();
}

void CalculateLoss(const vector<Tap>& chain, float main_light_level)
{
    // call title
    LossCalculationTitle();

    // initializations
    int footage;
    float splice_loss = 0.06 * 2;
    float bulkhead_loss = 0.3;

    // iterate through curent chain
    for (int i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // get footage from last point until this tap
        std::cout << "Enter footage for tap " << i + 1 << ": ";

        // receive footage for tap
        std::cin >> footage;
        float footage_loss = 0.0001 * footage;

        // ANSI escape codes
        // 033[F moves the cursor back to the start of the line
        // 033[K clears the line
        std::cout << "\033[F\033[K";

        // reprint everything since the line was cleared
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << "     | ";
        std::cout << t.tap_value_db;
        std::cout << "             | ";
        std::cout << footage;
        std::cout << "          | ";
        std::cout << main_light_level - footage_loss; // show the main level at the tap minus footage loss
        std::cout << "                   | ";
        std::cout << main_light_level - t.max_drop_loss - splice_loss - bulkhead_loss; // calculate loss at the drop 

        // update main level for loss between next tap
        float temp = main_light_level;
        temp = temp - t.max_insertion_loss;
        main_light_level = temp;

        std::cout << std::endl;
    }
}

void LightTable(const vector <Tap>& chain)
{
    // call title
    LightTableTitle();

    // iterate through the tap table
    for (const Tap& t : chain) 
    {
        // output the elements for each tap
        std::cout << t.port_count;
        std::cout << "     | ";
        std::cout << t.tap_value_db;
        std::cout << " dB          | ";
        std::cout << t.max_insertion_loss;
        std::cout << " dB         | ";
        std::cout << t.max_drop_loss;
        std::cout << " dB              | ";
        std::cout << (t.is_terminating ? "Yes" : "No");
        std::cout << std::endl;
    }

    // wait until user is done
    std::cout << std::endl;
    system("pause");

    // reset for cleanliness
    ResetTerminal();
}

int main()
{
    ResetTerminal();

    // initalize all taps and chain for the user
    vector<Tap> all_taps = populateTaps();
    vector<Tap> current_chain;

    // initialize, explain, and receive main light level
    float main_light_level;
    MainLightQuestion();
    std::cin >> main_light_level;

    ResetTerminal();
    
    // intialize menu choice
    int choice;
    do 
    {
        // menu for user to interact with
        MenuTitle();
        
        // get user choice
        std::cin >> choice;
        
        // choice 1 (call create)
        if (choice == 1)
        {
            // reset for cleanliness
            ResetTerminal();

            // clear current chain if it's not empty to save memory 
            if (!current_chain.empty())
            {
                CreationClear(current_chain);
            }
            current_chain = CreateNewChain(all_taps);
        }

        // choice (call view chain)
        else if (choice == 2)
        {
            // stop user from viewing if they havent created a chain
            if (current_chain.empty())
            {
                // sim processing
                std::cout << "\nPlease create a chain first. Resetting.";
                sleep(1);
                std::cout << ".";
                sleep(1);

                // reset for cleanliness
                ResetTerminal();
            }

            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                ViewChain(current_chain, main_light_level);
            }
        }
        
        // choice 3 (clear the chain)
        else if (choice == 3)
        {
            // stop user from clearing if they havent created a chain
            if (current_chain.empty())
            {
                // sim processing
                std::cout << "\nPlease create a chain first. Resetting.";
                sleep(1);
                std::cout << ".";
                sleep(1);

                // cleanliness
                ResetTerminal();
            }

            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                ClearChain(current_chain);
            }
        }

        // choice 4 (call tap insertion)
        else if (choice == 4)
        {
            // stop user from inserting if they havent created a chain
            if (current_chain.empty())
            {
                // sim processing
                std::cout << "\nPlease create a chain first. Resetting.";
                sleep(1);
                std::cout << ".";
                sleep(1);

                // cleanliness
                ResetTerminal();
            }

            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                InsertTap(current_chain, all_taps);
            }
        }

        // choice 5 (call tap replacement)
        else if (choice == 5)
        {
            // stop user from replacing if they havent created a chain
            if (current_chain.empty())
            {
                // sim processing
                std::cout << "\nPlease create a chain first. Resetting.";
                sleep(1);
                std::cout << ".";
                sleep(1);

                // cleanliness
                ResetTerminal();
            }     

            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                ReplaceTap(current_chain, all_taps);
            }
        }

        // choice 6 (call calculate loss)
        else if (choice == 6)
        {
            // stop user from calculating loss if they havent created a chain
            if (current_chain.empty())
            {
                // sim processing
                std::cout << "\nPlease create a chain first. Resetting.";
                sleep(1);
                std::cout << ".";
                sleep(1);

                // cleanliness
                ResetTerminal();
            }
            
            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                CalculateLoss(current_chain, main_light_level);
            }
        }

        // choice 7 (call light table)
        else if (choice == 7)
        {
            // reset for cleanliness and call func
            ResetTerminal();
            LightTable(all_taps);
        }

        // choice 8 (exit message)
        else if (choice == 8)
        {
            std::cout << "\nExiting...\n";
        }

        // anything else is wrong, retry & reset
        else
        {
            // sim processing
            std::cout << "\nInvalid choice! Resetting.";
            sleep(1);
            std::cout << ".";
            sleep(1);

            // cleanliness
            ResetTerminal();
        }

    } while (choice != 8); // do until exit (8) is choosen

    return 0;
}

/*
Notes
    Issues:
        Insertion is replacing a current tap instead of inserting in between (FIXED)
        Chain size being negative. Needs statement for making sure it's larger than 2
        While or for loop needed for tap value choice
        Terminal reset has problems with not functioning correctly when zooming in
        and when what's being written makes the terminal scroll
            ResetTerminal() needs to just reset everything written. look into behavior of function and library

    Things to add:
        High Priority:
            Need to start
                GUI
                When a tap value dB is entered that's outside port copunt parameters, dont go back to port count selection
                Tap reccomendation inside insertion and replacement functions

            In-progress
                Reseting/clearing terminal to show only what is being dealt with
                    IE: Menu being shown -> Option selected -> Menu is cleared from terminal history and shows interface of option
                    This would need signal handlers to go back to menu
                    Some functions need to pause and wait for user to be done viewing

            Done
                Main line light level prompt
                Light level at drops 
                Main line light level changing from tap to tap based off the table 
                Light level at drops chagning based off the table 
                Main line and drop light levels changing when tap inserted 
                Rework loss calculation 
                Insertion needs to refactor light loss on the whole chain *
                    * Calling calculate loss is working with inserted and replaced taps
                                Footage loss between taps; 0.0001 db/ft
                Insertion loss; approx 0.06 db/tap *
                    Needs to be checked for correct calculation
                Typical loss for bulk head; 0.2-0.3 db *
                    Needs to be checked for correct calculation
        
        Low priority:
            Make clearing the chain a function call (DONE)
            Clear temp chains used to insert and replace taps
            Delete tap function

Whitespace Fixes
    View Chain
        Needs to be handled to show until user wants to proceed,
        reset terminal, then proceed
    Invalid choice on menu
        Shows it's invalid -> Sleep -> ResetTerminal
    No chain made before using fetures
        Create function for checking that allows for similar output
        to invalid choice menu
            This allows the menu to stay at the top keeping functionality the same
*/