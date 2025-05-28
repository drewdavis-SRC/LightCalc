#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Tap 
{
    int port_count;
    int tap_value_db;
    double insertion_loss;
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

    cout << "\n=======================";
    cout << "\nChain creation selected";
    cout << "\n=======================";

    vector<Tap> chain;
    int num_taps;
    float main_light_level;

    if (main_light_level > 3.00 || main_light_level < 1.00)
    {
        cout << "\nYou've entered a number outside of the recommended range.\n";
        cout << "The program will continue. This is just a reminder.\n";
    }

    cout << "\nEnter number of taps in chain: ";
    cin >> num_taps;

    for (int i = 0; i < num_taps; i++)
    {
        bool is_last = (i == num_taps - 1);
        cout << "\nTap " << i + 1 << "/" << num_taps << ":\n";
        
        // Select port count
        int port_count;

        cout << "Enter port count (2/4/8): ";
        cin >> port_count;
        
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
            cout << "No valid taps! Retry.\n";
            i--;
            continue;
        }

        // Display options
        cout << "Available taps:\n";
        for (size_t j = 0; j < available.size(); j++) 
        {
            cout << j+1 << ". " << available[j].tap_value_db << " dB (Max Insertion Loss: " 
                 << available[j].insertion_loss << " dB)\n";
        }

        // Get selection
        int choice;

        cout << "Choose tap: ";
        cin >> choice;

        if (choice < 1 || choice > available.size()) 
        {
            cout << "Invalid! Retry.\n";
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
        cout << "\nChain is empty! Create one first.\n";
        return;
    }

    cout << "\n=======================";
    cout << "\nTap insertion selected";
    cout << "\n=======================";

    // Get position
    int position;
    cout << "\nInsert position (1-" << chain.size()+1 << "): ";
    cin >> position;

    if (position < 1 || position > chain.size()+1) 
    {
        cout << "Invalid position!\n";
        return;
    }
    position--; // Convert to 0-based index

    bool inserting_at_end = (position == chain.size());

    // Get port count
    int port_count;
    cout << "Port count (2/4/8): ";
    cin >> port_count;

    // Filter available taps
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
        cout << "No valid taps!\n";
        return;
    }

    // Select tap
    cout << "Available taps:\n";
    for (size_t i = 0; i < available.size(); i++) 
    {
        cout << i+1 << ". " << available[i].tap_value_db << " dB\n";
    }

    int choice;
    cout << "Choose tap: ";

    cin >> choice;
    if (choice < 1 || choice > available.size()) 
    {
        cout << "Invalid!\n";
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

    cout << "Tap inserted successfully!\n";
}

void ViewChain(const vector<Tap>& chain) {
    if (chain.empty()) 
    {
        cout << "\nChain is empty!\n";
        return;
    }

    cout << "\n=======================";
    cout << "\nChain viewing selected";
    cout << "\n=======================";

    cout << "\nCurrent Chain:\n";
    cout << "Pos | Ports | Tap (dB) | Max Ins Loss (dB) | Max Drop Loss (dB) | Terminating\n";
    cout << "----------------------------------------------------------------------------\n";
    
    for (size_t i = 0; i < chain.size(); i++) 
    {
        const Tap& t = chain[i];

        cout << t.port_count;
        cout << "     | ";
        cout << t.tap_value_db;
        cout << " dB          | ";
        cout << t.insertion_loss;
        cout << " dB                  | ";
        cout << t.max_drop_loss;
        cout << " dB           | ";
        cout << (t.is_terminating ? "Yes" : "No");
        cout << endl;
    }
}

double CalculateLoss(const vector<Tap>& chain) {

    cout << "\n===============================";
    cout << "\nTotal loss calculaiton selected";
    cout << "\n===============================";

    double total = 0.0;

    for (const Tap& t : chain) 
    {
        if (!t.is_terminating)
        {
            total += t.insertion_loss;
        }
    }

    cout << "\nTotal Insertion Loss: ";

    return total;
}

void LightTable (const vector <Tap>& tap)
{
    cout << "\n=========================";
    cout << "\nLight Table view selected";
    cout << "\n=========================";

    cout << "\nLight Loss Table:" << endl;
    cout << "Ports | Tap Value (dB) | Max Insertion Loss (dB) | Max Drop Loss (dB) | Terminating" << endl;
    cout << "-----------------------------------------------------------------------------------\n";

    for (const Tap& t : tap)
    {
        cout << t.port_count;
        cout << "     | ";
        cout << t.tap_value_db;
        cout << " dB          | ";
        cout << t.insertion_loss;
        cout << " dB                  | ";
        cout << t.max_drop_loss;
        cout << " dB           | ";
        cout << (t.is_terminating ? "Yes" : "No");
        cout << endl;
    }
}

int main ()
{
    vector<Tap> all_taps = populateTaps();
    vector<Tap> current_chain;

    int choice;

    float main_light_level;
    cout << "\nMain line light levels are usually 1-3 dB. This is needed to make proper calculations.";
    cout << "\nThe program is able to handle numbers up to 2 decimal places (IE: 2.45).\n";
    cout << "\nEnter the main line light level: ";
    cin >> main_light_level;

    do 
    {
        cout << "\n===== FTTH Tap Calculator Menu =====\n"
             << "1. Create New Chain\n"
             << "2. Insert Tap into Chain\n"
             << "3. View Light Loss Table\n"
             << "4. View Current Chain\n"
             << "5. Calculate Total Loss\n"
             << "6. Exit\n"
             << "====================================\n"
             << "\nChoice: ";
        
        cin >> choice;
        
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
            ViewChain(current_chain);
        }

        else if (choice == 5)
        {
            cout << CalculateLoss(current_chain) << " dB\n";
        }

        else if (choice == 6)
        {
            cout << "\nExiting...\n";
        }

        else
        {
            cout << "\nInvalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}