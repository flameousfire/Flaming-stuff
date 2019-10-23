#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

std::unordered_map<int, char> checknum
{
{0,	'0'},	{16, 'H'},
{1,	'1'},	{17,'J'},
{2,	'2'},	{18,'K'},
{3,	'3'},	{19,'L'},
{4,	'4'},	{20,'M'},
{5,	'5'},	{21,'N'},
{6,	'6'},	{22,'P'},
{7,	'7'},	{23,'R'},
{8,	'8'},	{24,'S'},
{9,	'9'},	{25,'T'},
{10, 'A'}, {26,'U'},
{11, 'B'}, {27,'V'},
{12, 'C'}, {28,'W'},
{13, 'D'}, {29,'X'},
{14, 'E'}, {30,'Y'},
{15, 'F'}
};

struct Person
{
    std::string fname;
    std::string lname;
    std::string age;
    std::string natiden;


    void printPerson()
    {
        std::cout << fname << " " << lname << "\n" << age << "\n" << natiden << "\n\n";
    }

    Person()
    {
        fname = "";
        lname = "";
        age = "-1";
        natiden = "";
    }

    void entername()
    {
        std::cout << "Please enter your name in the format 'firstname lastname': \n";
        std::string name = "";
        int tmp = -1;
        while (tmp == -1)
        {
            std::getline(std::cin, name);
            tmp = name.find_first_of(' ');
        }
        fname = name.substr(0, tmp);
        lname = name.substr(tmp + 1, name.size() - tmp);
    }

    void enterage()
    {
        std::cout << "Please enter your age: \n";
        age = "-1";
        while (std::stoi(age) < 0 || std::stoi(age) > 130)
        {
            std::cin >> age;
            if (std::stoi(age) < 0 || std::stoi(age) > 130)
                std::cout << "That's not a valid age. Try again: \n";
        }
    }

    void enterident()
    {
        std::cout << "Please enter your national identification number: \n";
        bool flag = false;
        while (flag == false)
        {
            std::cin >> natiden;
            int code = 0;
            try
                {code = (std::stoi(natiden.substr(0, 6)) * 1000 + std::stoi(natiden.substr(7, 3))) % 31;}
            catch(const std::exception& e)
            {
                std::cout << "Please try again. \n";
            }
            if (natiden.size() != 11)
                std::cout << "Wrong length: \n";
            else if (natiden[6] != 'A' && natiden[6] != '+' && natiden[6] != '-')
                std::cout << "Wrong separating symbol: \n";
            else if (natiden[7] == '9')
                std::cout << "This is a temporary id. number: \n";
            else if (checknum[code] != natiden[10])
                std::cout << "Your code character is wrong: \n";            
            else
                flag = true;   
        }
    }

    void enterpersondata()
    {
        entername();
        enterage();
        enterident();
    }

};
std::vector<Person> personlist = {};

bool checkident(std::string ident)
{
    for (int i = 0; i < personlist.size() - 1; ++i)
    {
        if (ident == personlist[i].natiden)
            return true;
    }
    return false;
}

std::string openfile()
{
    std::cout << "What file do you want to open?\n";
    std::string filename;
    std::cin >> filename;
    
    std::fstream rstream;
    rstream.open(filename, rstream.in);
    if (!rstream.is_open()) 
    {
        std::cerr << "Cannot open " << filename << "\n";
    }
    std::cout << "Do you want to open a new(n) work or add to current(c)?\n";
    char ans = ' ';
    while (ans != 'n' && ans != 'c')
        std::cin >> ans;
    if (ans == 'n')
    {
        personlist.empty();
    }

    rstream.seekp(0);
    while(!rstream.eof())
    {
        std::string readstr = "";
        std::getline(rstream, readstr);
        if (readstr == "")
            break;
        std::vector<std::string> line = {};
        std::string word = "";
        size_t pos = 0;
        while ((pos = readstr.find(',')) != std::string::npos)
        {
            std::string test = readstr.substr(0, pos);
            line.emplace_back(test);
            readstr.erase(0, pos + 1);
        }
        line.emplace_back(readstr);
        personlist.emplace_back(Person());

        personlist[personlist.size() - 1].fname = line[0];
        personlist[personlist.size() - 1].lname = line[1];
        personlist[personlist.size() - 1].age = line[2];
        personlist[personlist.size() - 1].natiden = line[3];   
        if (checkident(personlist[personlist.size()-1].natiden) == true)
        {
            personlist.pop_back();
        }
    }

    return filename;
}


int main()
{

    while (true)
    {
        std::vector<std::string> openfiles;
        std::cout << "Choose open(o) print(p), add(a), delete(d), edit(e), save(s) or exit(x).\n";
        char input = ' ';
        std::cin >> input;
        if (input == 'p')
        {   
            for (int i = 0; i < personlist.size(); ++i)
                personlist[i].printPerson();
        }
        else if (input == 'o')
            openfiles.emplace_back(openfile());
        else if (input == 'a')
        {
            personlist.emplace_back(Person());
            personlist[personlist.size()-1].enterpersondata();
            if (checkident(personlist[personlist.size()-1].natiden) == true)
            {
                std::cout << "That person already exits, no duplicates allowed.\n";
                personlist.pop_back();
            }
        }
        else if (input == 'd')
        {
            std::cout << "Which person do you want to delete?: \n";
            std::string name = "";
            int tmp = -1;
            while (tmp == -1)
            {
                std::getline(std::cin, name);
                tmp = name.find_first_of(' ');
            }
            std::string name1st = name.substr(0, tmp);
            std::string name2nd = name.substr(tmp + 1, name.size() - tmp);

            for (int i = 0; i < personlist.size(); ++i)
            {
                if (name1st == personlist[i].fname && name2nd == personlist[i].lname)
                {
                    std::cout << "Are you sure you want to delete this person? y/n \n";
                    personlist[i].printPerson();
                    char ans = ' ';
                    std::cin >> ans;
                    if (ans == 'n')
                        continue;
                    else if (ans == 'y')
                        personlist.erase(personlist.begin() + i);
                    else
                    {
                        std::cout << "Wrong input. \n";
                    }   
                }
            }
        }
        else if (input == 'e')
        {
            std::cout << "Whose info do you want to edit? \n";
            std::string name = "";
            int tmp = -1;
            while (tmp == -1)
            {
                std::getline(std::cin, name);
                tmp = name.find_first_of(' ');
            }
            std::string name1st = name.substr(0, tmp);
            std::string name2nd = name.substr(tmp + 1, name.size() - tmp);

            for (int i = 0; i < personlist.size(); ++i)
            {
                if (name1st == personlist[i].fname && name2nd == personlist[i].lname)
                {
                    personlist[i].printPerson();
                    std::cout << "What do you want to edit? Name(n), age(a) or identification number(i)? \n";
                    char input = ' ';
                    std::cin >> input;
                    if (input == 'n')
                        personlist[i].entername();
                    else if (input == 'a')
                        personlist[i].enterage();
                    else if (input == 'i')
                        personlist[i].enterident();
                    else
                    {
                            std::cout << "Not a valid action. \n";
                            continue;
                    }
                }
            }
        }
        else if (input == 'x')
            break;
        else if (input == 's')
        {   
            std::cout << "Currently open files are: ";
            for (int i = 0; i < openfiles.size(); ++i)
                std::cout << openfiles[i] << " ";
            std::cout << "\nChoose filename you want to save as (.txt): \n";
            std::string file;
            std::cin >> file;
            std::fstream wstream;
            wstream.open(file, wstream.out);
            for (int i = 0; i < personlist.size(); ++i)
            {
                wstream << personlist[i].fname << "," << personlist[i].lname << "," << personlist[i].age << "," << personlist[i].natiden << "\n";
            }
            wstream.close();
        }
    }


}