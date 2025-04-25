#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Tap {
    int port_count;
    int tap_value_db;
    double insertion_loss;
    bool is_terminating;
};

vector<Tap> populateTaps() {
    vector<Tap> taps = {
        // 2-port taps
        {2, 21, 0.25, false},
        {2, 19, 0.35, false},
        {2, 17, 0.40, false},
        {2, 15, 0.60, false},
        {2, 14, 0.80, false},
        {2, 12, 1.06, false},
        {2, 10, 1.69, false},
        {2, 8, 2.32, false},
        {2, 7, 3.58, false},
        {2, 5, 5.36, false},
        {2, 4, 0.00, true}, // terminating
        // 4-port taps
        {4, 21, 0.40, false},
        {4, 19, 0.60, false},
        {4, 17, 0.80, false},
        {4, 15, 1.06, false},
        {4, 13, 1.69, false},
        {4, 11, 2.32, false},
        {4, 10, 3.58, false},
        {4, 9, 5.36, false},
        {4, 7, 0.00, true}, // terminating
        // 8-port taps
        {8, 21, 0.60, false},
        {8, 19, 0.80, false},
        {8, 17, 1.06, false},
        {8, 15, 1.69, false},
        {8, 14, 2.32, false},
        {8, 12, 3.58, false},
        {8, 11, 0.00, true}, // terminating
    };
    return taps;
}

void LightTable(const vector<Tap>& taps) {
    cout << "Light Loss Table:\n";
    cout << "Port Count | Tap Value (dB) | Insertion Loss (dB) | Terminating\n";
    cout << "---------------------------------------------------------------\n";
    for (const Tap& t : taps) {
        cout << t.port_count << "         | " << t.tap_value_db << " dB        | " << t.insertion_loss << " dB            | " << (t.is_terminating ? "Yes" : "No") << endl;
    }
}

void TapInsertion(const vector<Tap>& taps) {
    int num_taps;
    cout << "Enter the number of taps in the chain: ";
    cin >> num_taps;

    if (num_taps < 1) {
        cout << "Invalid number of taps." << endl;
        return;
    }

    vector<const Tap*> selected_taps;
    for (int i = 0; i < num_taps; ++i) {
        bool is_last = (i == num_taps - 1);
        cout << "\nSelecting tap " << (i+1) << " of " << num_taps << endl;

        int port_count;
        cout << "Enter port count (2, 4, 8): ";
        cin >> port_count;
        if (port_count != 2 && port_count !=4 && port_count !=8) {
            cout << "Invalid port count. Please enter 2, 4, or 8." << endl;
            --i;
            continue;
        }

        vector<const Tap*> available;
        for (const Tap& t : taps) {
            if (t.port_count == port_count) {
                if (is_last) {
                    if (t.is_terminating) {
                        available.push_back(&t);
                    }
                } else {
                    if (!t.is_terminating) {
                        available.push_back(&t);
                    }
                }
            }
        }

        if (available.empty()) {
            cout << "No available taps for port count " << port_count << (is_last ? " (terminating)" : "") << endl;
            --i;
            continue;
        }

        cout << "Available tap values for " << port_count << "-port " << (is_last ? "(terminating):" : ":") << endl;
        for (size_t j = 0; j < available.size(); ++j) {
            const Tap* t = available[j];
            cout << (j+1) << ". " << t->tap_value_db << " dB";
            if (t->is_terminating) {
                cout << " (terminating)";
            }
            cout << ", Insertion loss: " << t->insertion_loss << " dB" << endl;
        }

        int choice;
        cout << "Enter your choice (1-" << available.size() << "): ";
        cin >> choice;
        if (choice < 1 || choice > available.size()) {
            cout << "Invalid choice." << endl;
            --i;
            continue;
        }

        const Tap* selected = available[choice - 1];
        selected_taps.push_back(selected);
    }

    double total_loss = 0.0;
    for (const Tap* t : selected_taps) {
        if (!t->is_terminating) {
            total_loss += t->insertion_loss;
        }
    }

    cout << "\nTotal insertion loss: " << total_loss << " dB" << endl;
}

int main() {
    vector<Tap> taps = populateTaps();

    cout << "\nThis program provides calculations for inserting new taps into the existing fiber infrastructure." << endl;
    cout << "Please follow the prompts carefully for accurate calculations." << endl;

    int choice;
    do {
        cout << "\nFeature Menu: " << endl;
        cout << "1. Tap insertion" << endl;
        cout << "2. Light loss table" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                TapInsertion(taps);
                break;
            case 2:
                LightTable(taps);
                break;
            case 3:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}