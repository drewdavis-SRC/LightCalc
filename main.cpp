#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>

using namespace std;

struct Tap 
{
    int tap_value;
    int tap_value_db;
    double max_insertion_loss;
    double max_drop_loss;
    bool is_terminating;
};

vector<Tap> populateTaps() 
{
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

vector<Tap> CreateNewChain(const vector<Tap>& all_taps) {

    std::cout << "\n=======================";
    std::cout << "\nChain Creation Selected";
    std::cout << "\n=======================";

    vector<Tap> chain;
    int num_taps;

    std::cout << "\n\nEnter number of taps in chain: ";
    std::cin >> num_taps;

    for (int i = 0; i < num_taps; i++)
    {
        bool is_last = (i == num_taps - 1);
        std::cout << "\nTap " << i + 1 << "/" << num_taps << ":\n";
        
        // Select port count
        int tap_value;

        std::cout << "Enter port count (2/4/8): ";
        std::cin >> tap_value;
        
        vector<Tap> available;
        for (const Tap& t : all_taps) 
        {
            if (t.tap_value == tap_value)
            {
                available.push_back(t);
            }
        }

        if (available.empty()) 
        {
            std::cout << "No valid taps! Retry.\n";
            i--;
            continue;
        }

        // Display options
        std::cout << "Available taps:\n";
        for (size_t j = 0; j < available.size(); j++) 
        {
            std::cout << j+1 << ". " << available[j].tap_value_db << " dB (Max Insertion Loss: " 
                 << available[j].max_insertion_loss << " dB)\n";
        }

        // Get selection
        int choice;

        std::cout << "\nChoose tap: ";
        std::cin >> choice;

        if (choice < 1 || choice > available.size()) 
        {
            std::cout << "Invalid! Retry.\n";
            i--;
            continue;
        }

        chain.push_back(available[choice-1]);
    }
    return chain;
}

void InsertTap(vector<Tap>& chain, const vector<Tap>& all_taps) {
    if (chain.empty())
    {
        std::cout << "\nChain is empty! Create one first.\n";
        return;
    }

    std::cout << "\n=======================";
    std::cout << "\nTap Insertion Selected";
    std::cout << "\n=======================";

    // Get position
    int position;
    std::cout << "\nInsert position (1-" << chain.size()+1 << "): ";
    std::cin >> position;

    if (position < 1 || position > chain.size()+1) 
    {
        std::cout << "Invalid position!\n";
        return;
    }
    position--; // Convert to 0-based index

    bool inserting_at_end = (position == chain.size());

    // Get port count
    int tap_value;
    std::cout << "Port count (2/4/8): ";
    std::cin >> tap_value;

    // Filter available taps
    vector<Tap> available;
    for (const Tap& t : all_taps) 
    {
        if (t.tap_value == tap_value)
        {
            available.push_back(t);
        }
    }

    if (available.empty()) 
    {
        std::cout << "No valid taps!\n";
        return;
    }

    // Select tap
    std::cout << "Available taps:\n";
    for (size_t i = 0; i < available.size(); i++) 
    {
        std::cout << i+1 << ". " << available[i].tap_value_db << " dB\n";
    }

    int choice;
    std::cout << "\nChoose tap: ";

    std::cin >> choice;
    if (choice < 1 || choice > available.size()) 
    {
        std::cout << "Invalid!\n";
        return;
    }
    Tap new_tap = available[choice-1];

    // Update previous last tap if inserting at end
    if (inserting_at_end && !chain.empty()) 
    {
        chain.back().is_terminating = false;
    }
    // Insert new tap
    chain.insert(chain.begin() + position, new_tap);

    std::cout << "Tap inserted successfully!\n";
    std::cout << "\nProceeding back to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
    std::cout << ".";
}

void ViewChain(const vector<Tap>& chain, float main_light_level) {
    if (chain.empty()) 
    {
        std::cout << "\nChain is empty!\n";
        return;
    }

    std::cout << "\n=======================";
    std::cout << "\nChain View Selected";
    std::cout << "\n=======================";

    std::cout << std::endl;
    std::cout << "Position | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB) | Terminating\n";
    std::cout << "----------------------------------------------------------------------------\n";
    
    for (size_t i = 0; i < chain.size(); i++) 
    {
        const Tap& t = chain[i];

        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.tap_value;
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

void CalculateLoss(const vector<Tap>& chain, float main_light_level) {

    std::cout << "\n===============================";
    std::cout << "\nLight Loss Calculaiton selected";
    std::cout << "\n===============================";
    std::cout << std::endl;

    std::cout << "Position | Tap Value (dB) | Main Light Level (dB) | Drop Light Level (dB)\n";
    std::cout << "-------------------------------------------------------------------------\n";

    for (int i = 0; i < chain.size(); i++)
    {
        const Tap& t = chain[i];

        std::cout << i + 1;
        std::cout << "        | ";
        std::cout << t.tap_value;
        std::cout << "              | ";
        std::cout << main_light_level;
        std::cout << "                   | ";
        std::cout << main_light_level - t.max_drop_loss;

        float temp = main_light_level;
        temp = temp - t.max_insertion_loss;
        main_light_level = temp;

        std::cout << std::endl;
    }
}

void LightTable (const vector <Tap>& chain)
{
    std::cout << "\n=========================";
    std::cout << "\nLight Table view selected";
    std::cout << "\n=========================";

    std::cout << std::endl;
    std::cout << "Ports | Tap Value (dB) | Max Insertion Loss (dB) | Max Drop Loss (dB) | Terminating" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------\n";

    for (const Tap& t : chain)
    {
        std::cout << t.tap_value;
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
}

int main ()
{
    vector<Tap> all_taps = populateTaps();
    vector<Tap> current_chain;

    int choice;

    float main_light_level;
    std::cout << "\nMain line light levels are usually 1-3 dB. This is needed to make proper calculations.";
    std::cout << "\nThe program is able to handle numbers up to 2 decimal places (IE: 2.45).\n";
    std::cout << "\nEnter the main line light level: ";
    std::cin >> main_light_level;

    std::cout << "\nProceeding to calculator menu.";
    sleep(1);
    std::cout << ".";
    sleep(1);
    std::cout << ".";
    sleep(1);
    
    do 
    {
        std::cout << "\n\n===== FTTH Tap Calculator Menu =====\n"
             << "1. Create New Chain\n"
             << "2. Insert Tap into Chain\n"
             << "3. View Light Loss Table\n"
             << "4. View Current Chain\n"
             << "5. Calculate Light Loss\n"
             << "6. Clear Current Chain\n"
             << "7. Exit\n"
             << "====================================\n"
             << "\nEnter your choice: ";
        
        std::cin >> choice;
        
        if (choice == 1)
        {
            current_chain = CreateNewChain(all_taps);
        }

        else if (choice == 2)
        {
            InsertTap(current_chain, all_taps);
        }

        else if (choice == 3)
        {
            LightTable(all_taps);
        }

        else if (choice == 4)
        {
            ViewChain(current_chain, main_light_level);
        }

        else if (choice == 5)
        {
            CalculateLoss(current_chain, main_light_level);
        }

        else if (choice == 6)
        {
            std::cout << "\n" << current_chain.size() << " taps found.\n";

            cout << "Deleting.";
            sleep(1);
            cout << ".";

            for (int i = 0; i <= current_chain.size(); i++)
            {
                current_chain.pop_back();
                sleep(1);
                cout << ".";
            }

            std::cout << "\nChain is now clear.\n";
            std::cout << "\nProceeding back to calculator menu.";
            sleep(1);
            std::cout << ".";
            sleep(1);
            std::cout << ".";
        }

        else if (choice == 7)
        {
            std::cout << "\nExiting...\n";
        }

        else
        {
            std::cout << "\nInvalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}

/*
Notes:

Issues:
    Insertion is replacing a current tap instead of inserting in between

Things to add:
    High Priority:
        GUI
        Main line light level prompt (done)
        Light level at drops (done)
        Main line light level changing from tap to tap based off the table
        Light level at drops chagning based off the table (done)
        Main line and drop light levels changing when tap inserted
        Rework loss calculation
        Insertion needs to refactor light loss on the whole chain
    
    Low priority:
        Reseting/clearing terminal to show only what is being dealt with
            IE: Menu being shown -> Option selected -> Menu is cleared from terminal history and shows interface of option
*/