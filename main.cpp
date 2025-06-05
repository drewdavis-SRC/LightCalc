#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>

using namespace std;

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

vector<Tap> CreateNewChain(const vector<Tap>& all_taps)
{

    std::cout << "\n=======================";
    std::cout << "\nChain Creation Selected";
    std::cout << "\n=======================";

    // initialize chain to store user selection
    vector<Tap> chain;

    // initialize and receive number of taps in the chain for iteration
    int num_taps;
    std::cout << "\n\nEnter number of taps in chain: ";
    std::cin >> num_taps;

    // create taps for the amount entered
    for (int i = 0; i < num_taps; i++)
    {
        // tell user which tap they're creating in the chain
        bool is_last = (i == num_taps - 1);
        std::cout << "\nTap " << i + 1 << "/" << num_taps << ":\n";
        
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

        // if the available chian is empty, showing in terminal
        if (available.empty()) 
        {
            std::cout << "No valid taps! Retry.\n";
            i--;
            continue;
        }


        // display tap options by iterating through available vector
        std::cout << "Available taps:\n";
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
            std::cout << "Invalid! Retry.\n";
            i--;
            continue;
        }

        // fill user chain with the choice
        chain.push_back(available[choice-1]);
    }
    // return the chain the user will be seeing
    return chain;
}

void InsertTap(vector<Tap>& chain, const vector<Tap>& all_taps)
{
    // stop user from inserting if they havent created a chain
    if (chain.empty())
    {
        std::cout << "\nPlease create a chain first.";
        return;
    }

    std::cout << "\n======================";
    std::cout << "\nTap Insertion Selected";
    std::cout << "\n======================";

    std::cout << "\nThe current chain is shown below.\n";
    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";

    // output taps for the entire current chain
    for (size_t i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // output tap elements
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

    // get position from user for insertion
    int position;
    std::cout << "\nInsert position (1-" << chain.size()+1 << "): ";
    std::cin >> position;

    // make sure position cannot be outside the range of the chain
    if (position < 1 || position > chain.size()+1) 
    {
        std::cout << "Invalid position!\n";
        return;
    }
    position--; // convert to 0-based index for vectors

    bool inserting_at_end = (position == chain.size());

    // get port count for insertion
    int port_count;
    std::cout << "Port count (2/4/8): ";
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

    if (available.empty()) 
    {
        std::cout << "No valid taps!\n";
        return;
    }

    // show all choices for insertion based off port count
    std::cout << "Available taps:\n";
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
        std::cout << "Invalid!\n";
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

    std::cout << "Tap inserted successfully!\n";
    std::cout << "\nProceeding back to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
}

void ReplaceTap (vector<Tap>& chain, const vector<Tap>& all_taps)
{
    // stop user from replacig if they havent created a chain
    if (chain.empty())
    {
        std::cout << "\nPlease create a chain first.";
        return;
    }

    std::cout << "\n========================";
    std::cout << "\nTap Replacement Selected";
    std::cout << "\n========================";
    std::cout << std::endl;

    std::cout << "\nThe current chain is shown below.\n";

    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB)\n";
    std::cout << "--------------------------------------------------------------------\n";

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

    // get the position for replacement
    int position;
    std::cout << "\nEnter the position of the tap to replace: ";
    std::cin >> position;

    // dont let position go out of range
    if (position < 1 || position > chain.size()+1) 
    {
        std::cout << "Invalid position!\n";
        return;
    }
    position--; // users see i + 1, move back to 0 based index for vectors

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
        std::cout << "No valid taps! Retry.\n";
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
        std::cout << "Invalid!\n";
        return;
    }

    // replace old tap with new
    chain[position] = available[choice - 1];

    std::cout << "Tap replaced successfully!\n";
    std::cout << "\nProceeding back to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
}

void ViewChain(const vector<Tap>& chain, float main_light_level)
{
    // stop user from viewing if they havent created a chain
    if (chain.empty()) 
    {
        std::cout << "\nPlease create a chain first.";
        return;
    }

    std::cout << "\n===================";
    std::cout << "\nChain View Selected";
    std::cout << "\n===================";
    std::cout << std::endl;

    std::cout << "\nPosition | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB) | Terminating\n";
    std::cout << "----------------------------------------------------------------------------------\n";
    
    // iterate through current chain
    for (size_t i = 0; i < chain.size(); i++) 
    {
        const Tap& t = chain[i];

        // output each element of the tap
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
}

void CalculateLoss(const vector<Tap>& chain, float main_light_level)
{
    // stop user from calculating if they havent created a chain
    if (chain.empty()) 
    {
        std::cout << "\nPlease create a chain first.";
        return;
    }

    std::cout << "\n===============================";
    std::cout << "\nLight Loss Calculaiton Selected";
    std::cout << "\n===============================";
    std::cout << std::endl;

    // initialize splice and bulkhead loss
    float splice_loss = 0.06 * 2;
    float bulkhead_loss = 0.3;

    // iterate through curent chain
    for (int i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        // initialize and receive footage for tap
        int footage;
        std::cout << "\nEnter the amout of line in feet leading up to tap " << i + 1 << ": ";
        std::cin >> footage;

        // initialize footage loss and calculate it
        float footage_loss = 0.0001 * footage;

        // subtract footage loss from main light level
        main_light_level = main_light_level - footage_loss;

        std::cout << "\nPosition | Ports | Tap Value (dB) | Footage | Main Light Level (dB) | Drop Light Level (dB)\n";
        std::cout << "---------------------------------------------------------------------------------\n";

        // output tap elements
        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.port_count;
        std::cout << "     | ";
        std::cout << t.tap_value_db;
        std::cout << "             | ";
        std::cout << main_light_level; // show the main level at the tap
        std::cout << "                   | ";
        std::cout << main_light_level - t.max_drop_loss - splice_loss - bulkhead_loss; // calculate loss at the drop 

        // update main level for loss between next tap
        float temp = main_light_level;
        temp = temp - t.max_insertion_loss;
        main_light_level = temp;

        std::cout << std::endl;
    }
}

void LightTable (const vector <Tap>& chain)
{
    std::cout << "\n=========================";
    std::cout << "\nLight Table View Selected";
    std::cout << "\n=========================";
    std::cout << std::endl;
    
    std::cout << "\nPorts | Tap Value (dB) | Max Insertion Loss (dB) | Max Drop Loss (dB) | Terminating" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------\n";

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
}

// not working
/* void ClearChain (const vector <Tap>& chain)
{
    std::cout << "\n" << chain.size() << " taps found.\n";

    std::cout << "Deleting.";
    sleep(1);
    std::cout << ".";

    for (int i = 0; i <= chain.size(); i++)
    {
        chain.pop_back();
        sleep(1);
        std::cout << ".";
    }

    std::cout << "\nChain is now clear.\n";
    std::cout << "\nProceeding back to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
} */

int main ()
{
    // initalize all taps and chain for the user
    vector<Tap> all_taps = populateTaps();
    vector<Tap> current_chain;

    // initialize, explain, and receive main light level
    float main_light_level;
    std::cout << "\nMain line light levels are usually 1-3 dB. This is needed to make proper calculations.";
    std::cout << "\nThe program is able to handle numbers up to 2 decimal places (IE: 2.45).\n";
    std::cout << "\nEnter the main line light level: ";
    std::cin >> main_light_level;

    std::cout << "\nProceeding to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
    
    // intialize menu choice
    int choice;
    do 
    {
        // menu for user to interact with
        std::cout << "\n\n===== FTTH Tap Calculator Menu =====\n"
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
        
        // get user choice
        std::cin >> choice;
        
        // choice 1 (call create)
        if (choice == 1)
        {
            // clear current chain if it's not empty to save memory 
            if (!current_chain.empty())
            {
                std::cout << "\nClearing previous chain with " << current_chain.size() << " taps.\n";

                std::cout << "Deleting.";
                sleep(1);
                std::cout << ".";

                for (int i = 0; i <= current_chain.size(); i++)
                {
                    current_chain.pop_back();
                    sleep(1);
                    std::cout << ".";
                }

                std::cout << "\nChain is now clear.\n";
                std::cout << "\nProceeding to chain cretion.";
                sleep(1);
                std::cout << ".";
                sleep(1);
            }
            current_chain = CreateNewChain(all_taps);
        }

        // choice (call view chain)
        else if (choice == 2)
        {
            ViewChain(current_chain, main_light_level);
        }
        
        // choice 3 (clear the chain)
        else if (choice == 3)
        {
            std::cout << "\n" << current_chain.size() << " taps found.\n";

            std::cout << "Deleting.";
            sleep(1);
            std::cout << ".";

            for (int i = 0; i <= current_chain.size(); i++)
            {
                current_chain.pop_back();
                sleep(1);
                std::cout << ".";
            }

            std::cout << "\nChain is now clear.\n";
            std::cout << "\nProceeding back to calculator menu.";
            sleep(1);
            std::cout << ".";
            sleep(1);
        }

        // choice 4 (call tap insertion)
        else if (choice == 4)
        {
            InsertTap(current_chain, all_taps);
        }

        // choice 5 (call tap replacement)
        else if (choice == 5)
        {
            ReplaceTap(current_chain, all_taps);
        }

        // choice 6 (call calculate loss)
        else if (choice == 6)
        {
            CalculateLoss(current_chain, main_light_level);
        }

        // choice 7 (call light table)
        else if (choice == 7)
        {
            LightTable(all_taps);
        }

        // choice 8 (exit message)
        else if (choice == 8)
        {
            std::cout << "\nExiting...\n";
        }

        // anything else is wrong, retry
        else
        {
            std::cout << "\nInvalid choice!\n";
        }

    } while (choice != 8); // do until exit (8) is choosen

    return 0;
}

/*
Notes
    Issues:
        Insertion is replacing a current tap instead of inserting in between (FIXED)

    Things to add:
        High Priority:
            Need to start
                GUI
                Footage loss between taps; 0.0001 db/ft
                Insertion loss; approx 0.06 db/tap
                Typical loss for bulk head; 0.2-0.3 db
                Reccomended taps
                When a tap value dB is entered that's outside port copunt parameters, dont go back to port count selection
                Tap reccomendation inside insertion and replacement functions

            In-progress

            Done
                Main line light level prompt (DONE)
                Light level at drops (DONE)
                Main line light level changing from tap to tap based off the table (DONE)
                Light level at drops chagning based off the table (DONE)
                Main line and drop light levels changing when tap inserted (DONE)
                Rework loss calculation (DONE)
                Insertion needs to refactor light loss on the whole chain (DONE*)
                    * Calling calculate loss is working with inserted and replaced taps
        
        Low priority:
            Reseting/clearing terminal to show only what is being dealt with
                IE: Menu being shown -> Option selected -> Menu is cleared from terminal history and shows interface of option
                This would need signal handlers to go back to menu
            Make clearing the chain a function call
            Clear temp chains used to insert and replace taps
*/