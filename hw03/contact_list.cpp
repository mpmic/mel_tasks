#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

namespace contact_list {

    bool add(storage &contacts, std::string_view name, number_t number) {

        if (name.empty()) {
            return false;
        }

        for (size_t i{0}; i < contacts.names.size(); ++i) {
            if (contacts.names[i] == name) {
                return false;
            }
        }

        contacts.names.push_back(static_cast<std::string>(name));
        contacts.numbers.push_back(number);
        return true;

    }

    size_t size(const storage &contacts) {
        return contacts.names.size();
    }

    number_t get_number_by_name(storage &contacts, std::string_view name) {

        for (size_t i{0}; i < contacts.names.size(); ++i) {
            if (contacts.names[i].compare(name) == 0) {
                return contacts.numbers[i];
            }
        }
        return -1;
    }

    std::string to_string(const storage &contacts) {

        std::string name;
        number_t number;
        std::string concatenated_string{""};

        for (size_t i{0}; i < contacts.names.size(); ++i) {
            name = contacts.names[i];
            number = contacts.numbers[i];

            concatenated_string.append(name);
            concatenated_string.append(" - ");
            concatenated_string.append(std::to_string(number));
            concatenated_string.append("\n");
        }

        return concatenated_string;
    }

    bool remove(storage &contacts, std::string_view name) {


        for (size_t i{0}; i < contacts.names.size(); ++i) {
            if (contacts.names[i] == name) {
                contacts.names.erase(std::next(contacts.names.begin(), i));
                contacts.numbers.erase(std::next(contacts.numbers.begin(), i));
                return true;
            }
        }
        return false;
    }

    void sort(storage &contacts) {

        struct contact {
            number_t number;
            std::string name;
        };

        std::vector <contact> contacts_zipped;

        for (size_t i{0}; i < contacts.names.size(); ++i) {
            contact temp_contact{.number=contacts.numbers[i], .name=contacts.names[i]};
            contacts_zipped.push_back(temp_contact);

        }

        std::sort(std::begin(contacts_zipped), std::end(contacts_zipped),
                  [](const contact &a, const contact &b) { return a.name.compare(b.name) < 0; });

        std::vector <number_t> sorted_numbers;
        std::vector <std::string> sorted_names;

        for (auto i: contacts_zipped) {
            sorted_names.push_back(i.name);
            sorted_numbers.push_back(i.number);
        }

        contacts.names = sorted_names;
        contacts.numbers = sorted_numbers;


    }

    std::string get_name_by_number(storage &contacts, number_t number) {

	  for (size_t i{0}; i < contacts.numbers.size(); ++i) {
            if (contacts.numbers[i] == number) {
                return contacts.names[i];
            }
        }
        return "";

    }

}