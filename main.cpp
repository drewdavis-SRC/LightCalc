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

void simulateF11()
{
    keybd_event(VK_F11, 0, 0, 0);
    keybd_event(VK_F11, 0, KEYEVENTF_KEYUP, 0);
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
    std::cout << "\nMain line light levels are usually 1-3 dB. This is needed to make proper calculations.";
    std::cout << "\nThe program is able to handle numbers up to 2 decimal places (IE: 2.45).\n";
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
             << "====================================\n";
}

void ChainCreationTitle()
{
    std::cout << "=======================";
    std::cout << "\nChain Creation Selected";
    std::cout << "\n=======================";
}

void ViewChainTitle()
{
    std::cout << "===================";
    std::cout << "\nChain View Selected";
    std::cout << "\n===================";
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

    std::cout << "\n\nThe current chain is shown below.";

    std::cout << "\n\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
}

void TapInsertionTaps (vector <Tap>& chain)
{
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
}

void TapReplacementTitle()
{
    std::cout << "========================";
    std::cout << "\nTap Replacement Selected";
    std::cout << "\n========================";

    std::cout << "\n\nThe current chain is shown below.";

    std::cout << "\n\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
}

void TapReplacementTaps(vector <Tap>& chain)
{
    // iterate through current chain for viewing
    for (size_t i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // show each element of the tap
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << " dB  | ";        
        std::cout << t.tap_value_db;
        std::cout << " dB    | ";
        std::cout << t.max_insertion_loss;
        std::cout << "               | ";
        std::cout << t.max_drop_loss;
        std::cout << std::endl;
    }
}

void LossCalculationTitle()
{
    std::cout << "===============================";
    std::cout << "\nLight Loss Calculation Selected";
    std::cout << "\n===============================";

    std::cout << "\n\nPosition | Ports | Tap Value (dB) | Footage (ft) | Main Light Level (dB) | Drop Light Level (dB)\n";
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
    // call title
    ChainCreationTitle();

    // initialize chain to store user selection
    vector<Tap> chain;

    // initialize int to store number of desired taps
    int num_taps;

    // start while loop to ask until desired output
    while (true)
    {
        // ask for and receive number of taps in the chain for iteration
        std::cout << "\n\nEnter number of taps in chain: ";
        std::cin >> num_taps;

        // make sure chain isnt smaller than 2
        if (num_taps < 2)
        {
            // reset terminal and call title again to display error above prompt
            ResetTerminal();
            ChainCreationTitle();

            // wrong msg
            std::cout << "\n\nERROR: Chain size must be 2 or larger. Retry.";
            continue;
        }
        // break desired input is given
        break;
    }
    // reset to save space in terminal
    ResetTerminal();

    // call the title again
    ChainCreationTitle();

    // reprint user choice
    std::cout << "\n\nEnter number of taps in chain: " << num_taps;

    // initialize tracker for user choice of tap to filter options through chain creation
    int previous_tap_value = 21; // start previous at 21 to be sure we're showing all taps to start

    // create taps for the amount entered
    for (int i = 0; i < num_taps; i++)
    {
        // tell user which tap they're creating in the chain
        bool is_last = (i == num_taps - 1);
        std::cout << "\n\n==== Tap " << i + 1 << "/" << num_taps << " ====\n";
        
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

            if (available[i].port_count == 4 && previous_tap_value < 11)
            {
                // reset terminal and call title to make error message be uniform
                ResetTerminal();
                ChainCreationTitle();

                std::cout << "\n\nERROR: The previous tap's dB level is too low to use that port.";

                // decrement i so we go until we correctly fill the correct element in the vector
                i--;

                // clear available vector
                available.clear();

                // continue loop
                continue;
            }

            if (available[i].port_count == 8 && previous_tap_value < 7)
            {
                // reset terminal and call title to make error message be uniform
                ResetTerminal();
                ChainCreationTitle();

                std::cout << "\n\nERROR: The previous tap's dB level is too low to use that port.";

                // decrement i so we go until we correctly fill the correct element in the vector
                i--;

                // clear available vector
                available.clear();

                // continue loop
                continue;
            }
        }

        // if the available chian is empty, show in terminal and tell user they picked an invalid port count
        if (available.empty()) 
        {
            // reset terminal and call title to make error message be uniform
            ResetTerminal();
            ChainCreationTitle();

            std::cout << "\n\nEnter number of taps in chain: " << num_taps;

            std::cout << "\n\nERROR: There are no vaild taps with that port count.";

            // decrement i so we go until we correctly fill the correct element in the vector
            i--;

            // continue loop
            continue;
        }

        // reset for cleanliness and call title
        ResetTerminal();
        ChainCreationTitle();

        // reprint user inputs and the tap they're filling
        std::cout << "\n\nEnter number of taps in chain: " << num_taps;
        std::cout << "\n\n==== Tap " << i + 1 << "/" << num_taps << " ====\n";
        std::cout << "Enter port count (2/4/8): " << port_count << std::endl;

        // initialize choice and start loop for desired input
        int choice;
        while (true)
        {
            // display tap options by iterating through available vector
            std::cout << "\nAvailable taps:\n";
            for (size_t j = 0; j < available.size(); j++) 
            {
                // only show taps that are less than or equal the previous tap db value for chain integrity 
                if (available[j].tap_value_db <= previous_tap_value)
                {
                    // output taps
                    std::cout << j + 1 << ". " << available[j].tap_value_db << " dB (Max Insertion Loss: " 
                        << available[j].max_insertion_loss << " dB)\n";
                }
            }
            
            std::cout << "\nChoose tap: ";
            std::cin >> choice;

            // if the user enters an invalid number
            if (choice < 1 || choice > available.size()) 
            {
                // reset terminal and call feature menu again
                ResetTerminal();
                ChainCreationTitle();

                std::cout << "\n\nEnter number of taps in chain: " << num_taps;

                std::cout << "\n\n==== Tap " << i + 1 << "/" << num_taps << " ====\n";
                std::cout << "Enter port count (2/4/8): " << port_count;

                // tell user they chose an invalid tap value
                std::cout << "\n\nERROR: Invalid tap value. Retry.\n";

                // continue loop
                continue;
            }
            // break if passes
            break;
        }

        // fill main chain with the user choice
        chain.push_back(available[choice-1]);

        // grab tap db value for checks
        previous_tap_value = available[i].tap_value_db;

        std::cout << "Check: " << previous_tap_value;

        // clear available vector
        available.clear();

        // reset the terminal and call the title again so we dont scroll on the terminal
        ResetTerminal();
        ChainCreationTitle();
        std::cout << "\n\nEnter number of taps in chain: " << num_taps;
    }

    // reset terminal and return the chain the user will be seeing
    ResetTerminal();

    // recall menu
    MenuTitle();

    // return created chain
    return chain;
}

void CreationClear(vector <Tap>& chain)
{
    // show how many taps will be cleared
    std::cout << "\nClearing previous chain with " << chain.size() << " taps.";
    sleep(1);
    std::cout << ".";
    sleep(1);

    // clear taps in the chain
    chain.clear();

    // output the chain is clear and pause another moment
    std::cout << "\n\nChain is now clear. Continuing.";
    sleep(2);
}

void ViewChain(const vector<Tap>& chain, float main_light_level)
{
    // call title for selection
    ViewChainTitle();

    // output columns and divider
    std::cout << "\n\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB) | Terminating\n";
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

    // recall menu
    MenuTitle();
}

void ClearChain(vector <Tap>& chain)
{
    // call feature's title
    ClearChainTitle();

    // show how many taps will be cleared
    std::cout << "\n\nClearing previous chain with " << chain.size() << " taps.";
    sleep(1);
    std::cout << ".";
    sleep(1);

    // clear taps in the chain
    chain.clear();

    // output the chain is clear
    std::cout << "\n\nChain is now clear.\n\n";

    // proceed when user is done
    system("pause");

    // reset for cleanliness
    ResetTerminal();

    // recall menu
    MenuTitle();
}

void InsertTap(vector<Tap>& chain, const vector<Tap>& all_taps)
{
    // call title
    TapInsertionTitle();
    TapInsertionTaps(chain);

    // initialize variable for user choice and begin loop
    int position;
    while (true)
    {
        // get position from user for insertion
        std::cout << "\nInsert position (1 - " << chain.size() + 1 << "): ";
        std::cin >> position;

        // make sure position cannot be outside the range of the chain
        if (position < 1 || position > chain.size() + 1) 
        {
            // reset terminal to get rid of old insertion question
            ResetTerminal();

            // call the title and taps again since they were deleted
            TapInsertionTitle();
            TapInsertionTaps(chain);
            
            // wrong input message
            std::cout << "\nERROR: Invalid position! Retry.\n";

            // continue loop
            continue;
        }

        // reset terminal and call everything above again
        // this is to get rid of error messages
        ResetTerminal();
        TapInsertionTitle();
        TapInsertionTaps(chain);

        // show previous choices
        std::cout << "\nInsert position (1 - " << chain.size() + 1 << "): " << position << "\n";

        // break if passes
        break;
    }

    // convert to 0-based index for vectors so we are inserting at the correct place 
    position--;

    // make sure we remember if a tap is the end of a chain
    bool inserting_at_end = (position == chain.size());

    // make available vector
    vector<Tap> available;

    // get port count for insertion
    int port_count;
    while (true)
    {
        std::cout << "\nPort count (2/4/8): ";
        std::cin >> port_count;

        // filter available taps based off user decision
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
            // clear available chain since we're calling it again
            available.clear();

            // reset terminal and call everything above again
            ResetTerminal();
            TapInsertionTitle();
            TapInsertionTaps(chain);

            // show previous position that was chosen
            std::cout << "\nInsert position (1 - " << chain.size() + 1 << "): " << position + 1 << "\n";

            // wrong input message
            std::cout << "\nERROR: No valid taps with that port count! Retry.\n";

            // continue loop
            continue;
        }

        // reset terminal and call everything above again
        // this is to get rid of error messages
        ResetTerminal();
        TapInsertionTitle();
        TapInsertionTaps(chain);

        // show previous choices
        std::cout << "\nInsert position (1 - " << chain.size() + 1 << "): " << position + 1 << "\n";
        std::cout << "\nPort count (2/4/8): " << port_count << "\n";

        // leave loop once valid port count is 
        break;
    }

    // get tap value choice
    int choice;
    while (true)
    {
        // show all choices for insertion based off port count
        std::cout << "\nAvailable taps:\n";
        for (size_t i = 0; i < available.size(); i++) 
        {
            std::cout << i + 1 << ". " << available[i].tap_value_db << " dB (Max Insertion Loss: " 
                    << available[i].max_insertion_loss << " dB)\n";;
        }

        std::cout << "\nChoose tap: ";
        std::cin >> choice;

        // dont let the choice be out of range
        if (choice < 1 || choice > available.size()) 
        {
            // reset terminal and call everything above again
            ResetTerminal();
            TapInsertionTitle();
            TapInsertionTaps(chain);

            // show previous position and port count that were chosen
            std::cout << "\nInsert position (1 - " << chain.size() + 1 << "): " << position + 1 << "\n";
            std::cout << "\nPort count (2/4/8): " << port_count << "\n";

            // wrong input message
            std::cout << "\nERROR: Invalid choice! Retry.\n";

            // continue loop
            continue;
        }
        // break if passes
        break;
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

    // show new chain
    std::cout << "\n\nUpdated chain:";
    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
    TapInsertionTaps(chain);
    std::cout << std::endl;

    // clear available vector
    available.clear();

    // proceed when user is done
    system("pause");

    // reset for cleanliness
    ResetTerminal();

    // recall menu
    MenuTitle();
}

void ReplaceTap(vector<Tap>& chain, const vector<Tap>& all_taps)
{
    TapReplacementTitle();
    TapReplacementTaps(chain);

    // initialize variable for user choice and begin loop
    int position;
    while (true)
    {
        // get position from user for replacement
        std::cout << "\nReplace position (1 - " << chain.size() << "): ";
        std::cin >> position;

        // make sure position cannot be outside the range of the chain
        if (position < 1 || position > chain.size()) 
        {
            // reset terminal to get rid of old replacement question
            ResetTerminal();

            // call the title and taps again since they were deleted
            TapReplacementTitle();
            TapReplacementTaps(chain);
            
            // wrong input message
            std::cout << "\nERROR: Invalid position! Retry.\n";

            // continue loop
            continue;
        }

        // reset terminal and call everything above again
        // this is to get rid of error messages
        ResetTerminal();
        TapReplacementTitle();
        TapReplacementTaps(chain);

        // show previous choices
        std::cout << "\nReplace position (1 - " << chain.size() << "): " << position << "\n";

        // break if passes
        break;
    }
    position--; // users see i + 1, move back to 0 based index for vectors for correct replacement

    // fill available vector of taps with that port count
    vector<Tap> available;

    // get the port count of replacement tap
    int port_count;
    while (true)
    {
        std::cout << "\nEnter port count (2/4/8): ";
        std::cin >> port_count;

        // fill available vector of taps with that port count
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
            // clear available chain since we're calling it again
            available.clear();

            // reset terminal and call everything above again
            ResetTerminal();
            TapReplacementTitle();
            TapReplacementTaps(chain);

            // show previous position that was chosen
            std::cout << "\nReplace position (1 - " << chain.size() << "): " << position + 1 << "\n";

            // wrong input message
            std::cout << "\nERROR: No valid taps with that port count! Retry.\n";

            // continue loop
            continue;
        }
        
        // reset terminal and call everything above again
        // this is to get rid of error messages
        ResetTerminal();
        TapReplacementTitle();
        TapReplacementTaps(chain);

        // show previous choices
        std::cout << "\nReplace position (1 - " << chain.size() << "): " << position + 1 << "\n";
        std::cout << "\nPort count (2/4/8): " << port_count << "\n";

        // leave loop once valid port count is 
        break;
    }

    // initialize variable for user choice and start loop
    int choice;
    while (true)
    {
        // show all choices for replacement based off port count
        std::cout << "\nAvailable taps:\n";
        for (size_t i = 0; i < available.size(); i++) 
        {
            std::cout << i + 1 << ". " << available[i].tap_value_db << " dB (Max Insertion Loss: " 
                    << available[i].max_insertion_loss << " dB)\n";;
        }

        // get user choice
        std::cout << "\nChoose tap: ";
        std::cin >> choice;

        // dont let the choice be out of range
        if (choice < 1 || choice > available.size()) 
        {
            // reset terminal and call everything above again
            ResetTerminal();
            TapReplacementTitle();
            TapReplacementTaps(chain);

            // show previous position and port count that were chosen
            std::cout << "\nReplace position (1 - " << chain.size() << "): " << position + 1 << "\n";
            std::cout << "\nPort count (2/4/8): " << port_count << "\n";

            // wrong input message
            std::cout << "\nERROR: Invalid choice! Retry.\n";

            // continue loop
            continue;
        }

        // break if passes
        break;
    }
    // replace old tap with new
    chain[position] = available[choice - 1];

    // show user replacement was successful
    std::cout << "\nTap replaced successfully!";

    // show new chain
    std::cout << "\n\nUpdated chain:";
    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";
    TapReplacementTaps(chain);
    std::cout << std::endl;

    // clear available vector
    available.clear();

    // proceed when user is done
    system("pause");

    // reset for cleanliness
    ResetTerminal();

    // recall menu
    MenuTitle();
}

vector<int> CalculateLoss(const vector<Tap>& chain, float main_light_level, vector<int>& FootageSave)
{
    // call title
    LossCalculationTitle();

    // initializations
    int footage;
    float splice_loss = 0.06 * 2;
    float bulkhead_loss = 0.3;
    float attenuation;
    char ans;
    int size_check = 0;

    // ANSI escape codes
    // 033[F moves the cursor back to the start of the line
    // 033[K clears the line

    // clearing table delineations for cleanliness
    std::cout << "\033[F\033[K";
    std::cout << "\033[F\033[K";
    
    while (true)
    {
        std::cout << "Are you calculating for a new/updated/altered chain? (y/n): ";
        std::cin >> ans;

        // get rid of question and recall title
        ResetTerminal();
        LossCalculationTitle();

        if (ans == 'y')
        {
            // make sure we dont do calculations on a chain that hasn't been altered
            if (FootageSave.size() == size_check)
            {
                ResetTerminal();
                LossCalculationTitle();

                // clearing table delineations for cleanliness
                std::cout << "\033[F\033[K";
                std::cout << "\033[F\033[K";

                std::cout << "ERROR: This chain has not been changed since the last calculation.\n";
                std::cout << "Please alter or clear the chain first.\n\n";
                continue;
            }

            // clear footage save in case chain has been altered
            if (!FootageSave.empty())
            {
                FootageSave.clear();
            }

            // iterate through curent chain
            for (int i = 0; i < chain.size(); i++)
            {
                const Tap& t = chain[i];

                // get footage from last point until this tap
                std::cout << "Enter footage for tap " << i + 1 << ": ";

                // receive footage for tap
                std::cin >> footage;
                FootageSave.insert(FootageSave.begin() + i, footage);

                float footage_loss = 0.0001 * footage;

                // ANSI escape codes
                // 033[F moves the cursor back to the start of the line
                // 033[K clears the line
                std::cout << "\033[F\033[K";

                // print all table values
                std::cout << i + 1;
                std::cout << "        | ";
                std::cout << t.port_count;
                std::cout << "     | ";
                std::cout << t.tap_value_db;
                std::cout << "             | ";
                std::cout << footage;

                // calculate the attenuation on the main light level from the footage loss
                attenuation = main_light_level - footage_loss;

                std::cout << "          | ";
                std::cout << attenuation; // show the main level at the tap minus footage loss
                std::cout << "                 | ";
                std::cout << attenuation - t.max_drop_loss - splice_loss - bulkhead_loss; // calculate loss at the drop 

                // update main level for loss between next tap and the insertion loss
                main_light_level = attenuation - t.max_insertion_loss;

                std::cout << std::endl;

                // update size check
                size_check = size_check + 1;
            }
            // wait for user to be done viewing
            std::cout << std::endl;
            system("pause");

            // remove everything from terminal for cleanliness
            ResetTerminal();

            // recall menu
            MenuTitle();
        }

        else if (ans == 'n')
        {
            if (FootageSave.empty())
            {
                ResetTerminal();
                LossCalculationTitle();

                // clearing table delineations for cleanliness
                std::cout << "\033[F\033[K";
                std::cout << "\033[F\033[K";

                std::cout << "ERROR: This is a new/altered chain.\n";
                std::cout << "Please select 'y' before selecting 'n'.\n\n";
                continue;
            }

            // iterate through curent chain
            for (int i = 0; i < chain.size(); i++)
            {
                const Tap& t = chain[i];

                footage = FootageSave[i];

                float footage_loss = 0.0001 * footage;

                // print values
                std::cout << i + 1;
                std::cout << "        | ";
                std::cout << t.port_count;
                std::cout << "     | ";
                std::cout << t.tap_value_db;
                std::cout << "             | ";
                std::cout << footage;

                // calculate the attenuation on the main light level from the footage loss
                attenuation = main_light_level - footage_loss;

                std::cout << "          | ";
                std::cout << attenuation; // show the main level at the tap minus footage loss
                std::cout << "                 | ";
                std::cout << attenuation - t.max_drop_loss - splice_loss - bulkhead_loss; // calculate loss at the drop 

                // update main level for loss between next tap and the insertion loss
                main_light_level = attenuation - t.max_insertion_loss;

                std::cout << std::endl;

                // update size check
                size_check = size_check + 1;
            }

            // wait for user to be done viewing
            std::cout << std::endl;
            system("pause");

            // remove everything from terminal for cleanliness
            ResetTerminal();

            // recall menu
            MenuTitle();
        }

        else
        {
            ResetTerminal();
            LossCalculationTitle();

            // clearing table delineations for cleanliness
            std::cout << "\033[F\033[K";
            std::cout << "\033[F\033[K";

            std::cout << "Invlaid entry. Try again.\n\n";
            continue;
        }

        // leave once user correctly chooses
        break;
    }

    return FootageSave;
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
        std::cout << " dB                  | ";
        std::cout << t.max_drop_loss;
        std::cout << " dB           | ";
        std::cout << (t.is_terminating ? "Yes" : "No");
        std::cout << std::endl;
    }

    // wait until user is done
    std::cout << std::endl;
    system("pause");

    // reset for cleanliness
    ResetTerminal();

    // recall menu
    MenuTitle();
}

int main()
{
    // make program start in fullscreen
    simulateF11();

    // reset incase program is run from vs code terminal
    ResetTerminal();

    // initialize main light level
    float main_light_level;

    std::cout << "** NOTES: - Fullscreen is being used for the best performance.\n"
                << "          - Press 'CTL + C' to force quit at any time.\n"
                << "          - Press 'F11' to remove fullscreen mode.\n";

    // main light level info
    MainLightQuestion();
    while (true)
    {
        // receive main light level
        std::cout << "\nEnter the main line light level: ";
        std::cin >> main_light_level;

        // check input and allow some leeway
        if (main_light_level > 3.5 || main_light_level < 1)
        {
            // reset terminal if wrong input to get error message above input entry
            ResetTerminal();

            // reprint info
            std::cout << "** NOTES: - Fullscreen is being used for the best performance.\n"
                << "          - Press 'CTL + C' to force quit at any time.\n"
                << "          - Press 'F11' to remove fullscreen mode.\n";
            MainLightQuestion();

            // error msg and continue loop
            std::cout << "\nMain light levels must be greater than 1 and less than 3.5.\n";
            continue;
        }
        // break if passes
        break;
    }

    ResetTerminal();

    // initalize all taps and chain for the user
    vector<Tap> all_taps = populateTaps();
    vector<Tap> current_chain;
    vector<int> FootageSave;
    
    // intialize menu choice
    int choice;

    // menu for user to interact with
    MenuTitle();
        
    while (true)
    {
        std::cout << "\nEnter your choice: ";
        // get user choice
        std::cin >> choice;
        
        // choice 1 (call create)
        if (choice == 1)
        {
            // clear current chain if it's not empty to save memory 
            if (!current_chain.empty())
            {
                CreationClear(current_chain);
            }

            // reset for cleanliness
            ResetTerminal();

            // create new chain from all taps
            current_chain = CreateNewChain(all_taps);
        }

        // choice (call view chain)
        else if (choice == 2)
        {
            // stop user from viewing if they havent created a chain
            if (current_chain.empty())
            {
                ResetTerminal();
                MenuTitle();
                std::cout << "\nPlease create a chain first.\n";
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
                ResetTerminal();
                MenuTitle();
                std::cout << "\nPlease create a chain first.\n";
                
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
                ResetTerminal();
                MenuTitle();
                std::cout << "\nPlease create a chain first.\n";
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
                ResetTerminal();
                MenuTitle();
                std::cout << "\nPlease create a chain first.\n";
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
                ResetTerminal();
                MenuTitle();
                std::cout << "\nPlease create a chain first.\n";
            }
            
            else
            {
                // reset for cleanliness and call func
                ResetTerminal();
                CalculateLoss(current_chain, main_light_level, FootageSave);
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
            // clear vectors before ending
            current_chain.clear();
            all_taps.clear();

            // exit message
            std::cout << "\nExiting...\n";
            break;
        }

        // anything else is wrong, retry & reset
        else
        {
            ResetTerminal();
            MenuTitle();
            std::cout << "\nInvalid menu selection!\n";
        }
    }

    return 0;
}

/*
Notes
    This is made to compile on windows systems only

    Issues:
        Fixed:
            Insertion is replacing a current tap instead of inserting in between

            View Chain
                Needs to be handled to show until user wants to proceed,
                reset terminal, then proceed

            Invalid choice on menu
                Shows it's invalid -> Sleep -> ResetTerminal

            No chain made before using features
                Create function for checking that allows for similar output
                to invalid choice menu
                    This allows the menu to stay at the top keeping functionality the same

            Start program in fullscreen

            Chain size being negative. Needs statement for making sure it's larger than 2

            Chain creation clear message needs to be below chain creation title
        
        Not Fixed:
            For large chains, users should not be able to select a port count that does not have any valid taps
                Needs to have a message that shows X port count is not available because the chain has gone past the min db value
                needed to use that port
                Needs to remove the port option from the selection
                EX: Tap db values have gone underneath 11, so 8 port should not be shown or available to choose;
                    Same goes for 4 port taps; when underneath 7, remove 4 port option
                Currently, checking previous db values filters out taps when choosing a port in this manner, but the available
                vector is empty and shows nothing.

            Terminal reset has problems with not functioning correctly when zooming in
            and when what's being written makes the terminal scroll
                ResetTerminal() needs to just reset everything written. look into behavior of function and library

            Main Light Level is resetting back to first input after first tap

    Things to add:
        High Priority:
            Need to start
                GUI

                Tap reccommendation
                    Could be standalone feature or inside insertion and replacement functions

            In-progress
                Save footage entered from user for ease of use
                    Implemented; Testing needs to take place

            Done
                When a tap value dB is entered that's outside port count parameters, dont go back to port count selection
                    While loop needed

                Main line light level prompt

                Light level at drops 

                Main line light level changing from tap to tap based off the table 

                Light level at drops changing based off the table 

                Main line and drop light levels changing when tap inserted 

                Rework loss calculation 
                
                Insertion needs to refactor light loss on the whole chain *
                    * Calling calculate loss is working with inserted and replaced taps
                                Footage loss between taps; 0.0001 db/ft

                Insertion loss; approx 0.06 db/tap *
                    Needs to be checked for correct calculation

                Typical loss for bulk head; 0.2-0.3 db *
                    Needs to be checked for correct calculation

                Reseting/clearing terminal to show only what is being dealt with
                    IE: Menu being shown -> Option selected -> Menu is cleared from terminal history and shows interface of option
                    This would need signal handlers to go back to menu
                    Some functions need to pause and wait for user to be done viewing
        
        Low priority:
            Need to start
                Delete tap function

            In-progress

            Done
                Clear temp chains used to insert and replace taps

                Make clearing the chain a function call
*/