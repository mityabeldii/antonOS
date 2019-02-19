// createed by dmytro beldii, 2018
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fstream>
#include <grp.h>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <pwd.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>
#include <vector>
#include <fcntl.h>
#include <typeinfo>
using namespace std;

string request = "cp 1.txt 2.txt -p \n";

string help = "usage: cp [-R [-H | -L | -P]] [-fi | -n] [-apvXc] source_file target_file\n       cp [-R [-H | -L | -P]] [-fi | -n] [-apvXc] source_file ... target_directory";

bool copyFile(const char *SRC = "", const char *DEST = "")
{
    ifstream src(SRC, ios::binary);
    ofstream dest(DEST, ios::binary);
    dest << src.rdbuf();
    return src && dest;
}

int getPermission(const char *path, char letter)
{
    if (letter == 'r')
    {
        if (access(path, R_OK) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if (letter == 'w')
    {
        if (access(path, W_OK) == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

vector<string> split(const string &sentence = "")
{
    istringstream iss(sentence);
    vector<string> tokens;
    copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(tokens));
    return tokens;
}

vector<string> readRequest()
{
    // get user name
    register struct passwd *pw;
    register uid_t uid;
    int c;
    uid = geteuid ();
    pw = getpwuid (uid);
    // print intro
    string cyan = "\033[1;36m", reset = "\033[0m";
    cout << cyan << pw->pw_name << reset << "@antonOS: $ ";
    getline(cin, request);
    return split(request);
}

int exeption()
{
    const char *cstr = request.c_str();
    cout << ">> run exeption for \"" << request << "\"" << endl;
    system(cstr);
    return 0;
}

int error(string comment = "")
{
    cout << ">> error: " << comment << endl;
    return 0;
}

int flag_h()
{
    cout << help << endl;
    return 0;
}

int flag_v()
{
    cout << split(request)[1] << " >> " << split(request)[2] << endl;
    return 0;
}

bool flag_i()
{
    string confirm = "";
    cout << "Are you sure? (Y/n)" << endl;
    getline(cin, confirm);
    while (!(!confirm.compare("Y") || !confirm.compare("n")))
    {
        cout << "(Y/n)" << endl;
        getline(cin, confirm);
    }
    return confirm.compare("Y");
}

int getMetadata() {
    return 0;
}

int ditto() {
    // macos interpretetion for -p flag, but also works with Ubuntu
    string set = "ditto ";
    set.append(split(request)[1]);
    set.append(" ");
    set.append(split(request)[2]);
    system(set.c_str());
    return 0;
}

int cp()
{
    vector<string> splitedRequest = split(request);
    if (splitedRequest.size() >= 3)
    {
        // if format is correct
        if (splitedRequest.size() == 3)
        {
            // if there are no flags
            copyFile(splitedRequest[1].c_str(), splitedRequest[2].c_str());
        }
        else
        {
            // with flags
            for (int i = 3; i < split(request).size(); i++)
            {
                string flag = split(request)[i];
                if ((flag.compare("-h") == 0) || (flag.compare("-help") == 0))
                {
                    flag_h();
                    return 0;
                }
                if ((flag.compare("-v") == 0) || (flag.compare("-–verbose") == 0))
                {
                    flag_v();
                    copyFile(splitedRequest[1].c_str(), splitedRequest[2].c_str());
                }
                if ((flag.compare("-i") == 0) || (flag.compare("-–interactive") == 0))
                {
                    bool acc = flag_i();
                    if (acc == 0)
                    {
                        copyFile(splitedRequest[1].c_str(), splitedRequest[2].c_str());
                    }
                }
                if ((flag.compare("-f") == 0) || (flag.compare("-–force") == 0))
                {
                    int access = getPermission(split(request)[2].c_str(), 'w');
                    if (access == 0)
                    {
                        remove(split(request)[2].c_str());
                    }
                    copyFile(splitedRequest[1].c_str(), splitedRequest[2].c_str());
                }
                if ((flag.compare("-p") == 0) || (flag.compare("-–preserve") == 0))
                {
                    getMetadata();
                    utime("./2.txt", NULL);
                    ditto();
                }
            }
        }
    }
    else
    {
        error("format is not correct; run 'cp path path -h' for help");
    }
    return 0;
}

int revert()
{
    string revert = "";
    cout << "Done" << endl << "Do you want to revert copieng? (y/N)" << endl;
    getline(cin, revert);
    if (revert == "y")
    {
        string del = "rm -rf ";
        del.append(split(request)[2]);
        remove(del.c_str());
        cout << "...reverted" << endl;
    }
    return 0;
}

int cycle()
{
    readRequest();

    // S_IRWXU - rw
    // S_IRUSR - r-
    // S_IWUSR - -w
    // S_IXUSR - --
    // chmod(split(request)[2].c_str(), S_IRWXU);

    if (request == "") {
        return cycle();
    }

    if (split(request)[0] == "cp")
    {
        cp();
        // revert();
    }
    else
    {
        // error("command is not found");
        exeption();
    }

    cycle();

    return 0;
}

int main()
{

    cout << endl << "Welcome to antonOS!" << endl << "OS version: 0.3.2" << endl << endl;
    cout << " - note: \"command is not found\" means that OS is not filled with this command" << endl;
    cout << " - note: \"run exeption\" means that OS is using inner Linux tools to finish compilaiton with success" << endl;
    cout << endl;
    cycle();

    return 0;
}