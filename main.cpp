#include <iostream>
#include <GL/glut.h>
#include <bits/stdc++.h>
#include <random>

#define endl "\n"
#define WIDTH_SHIFT 0.8

using namespace std;

// +13 is representing upper limit of square wave form
// -13 is representing lower limit of square wave form
// 100 is representing 0 in square wave form

// Global variables
int bit_seq_len_global; // stores the length of generated bit stream.
string long_pal_str; // stores the longest palindromic substring.
vector<int> bit_seq_gen_global; // stores the bit stream (0's & 1's)
vector<int> bit_seq_encoded_global; // stores the voltage level from graph used for decoding signal back to bit stream.

int firstMenu()
{
    int data_gen_choice;
    cout<<"\t\t\tPLEASE SELECT DATA GENERATION SCHEME!"<<endl;
    cout<<"\t1.Random Sequence"<<endl;
    cout<<"\t2.Random Sequence containing fixed sub-sequence"<<endl;
    cout<<"\t3.Exit"<<endl;
    cout<<"\t\t\tPRESS 1 for choice 1, 2 for choice 2 and 3 for exit"<<endl;
    cin>>data_gen_choice;
    return data_gen_choice;
}

int sub_seq_menu()
{
    int sub_seq_choice;
    cout<<"\t\t\t PLEASE CHOOSE SUB-SEQUENCE SCHEME"<<endl;
    cout<<"\t1.Eight consecutive 0's - xxx00000000xx"<<endl;
    cout<<"\t2.Four consecutive 0's - xxx0000xxx"<<endl;
    cout<<"\t3.Exit"<<endl;
    cout<<"\t\t\tPRESS 1 for choice 1, 2 for choice 2 and 3 for exit"<<endl;
    cin>>sub_seq_choice;
    return sub_seq_choice;
}

vector<int> randomSeqSubSeqGen(int bit_seq_len, int consc_zero_choice)
{
    // count of zero will be four if user opted for choice 2 i.e. 4 0's
    int zero_count = 4;
    // count of zero will be eight if user opted for choice 1 i.e. 8 0's
    if(consc_zero_choice == 1) zero_count = 8;
    // will create a vector with fixed bit_seq length with all the values initially set to 5.
    vector<int> bit_seq(bit_seq_len, 5);
    srand(time(NULL));
    // will select a random position to insert the subsequence.
    int insert_posn = rand()%bit_seq_len;
    while(zero_count--) // will run the loop until all the 0's have been inserted to bit stream.
    {
        // if the subseq length is greater then stream length then we cannot insert subseq and we have to take complete random seq.
        if(bit_seq_len-zero_count<0) {
            cout << "\n\n\n\n\n\n\t\t\t!!!!Zero_count: "<<zero_count<<" is greater the bit_seq length: "<<bit_seq_len<<" so we will create a random sequence."<<endl;
            break;
        }
        bit_seq[insert_posn] = 0;
        insert_posn = (insert_posn+1)%bit_seq_len; // will increase the insert position one by one treating the array as circular array.
    }
    // will set the remaining bits to 0 or 1 randomly.
    for(int i = 0; i <bit_seq_len; i++)
        if (bit_seq[i] == 5) bit_seq[i] = rand()%2;
    return bit_seq;
}

vector<int> randomSeqGen(int bit_seq_len)
{
    vector<int> bit_seq;
    srand(time(NULL));
    while(bit_seq_len--) bit_seq.push_back(rand()%2);
    return bit_seq;
}

void bit_seq_display(vector<int> bit_seq)
{
    for (int i=0; i<bit_seq.size(); i++)
    cout<<bit_seq[i];
}

void bitSeqChoice()
{
    int data_gen_choice = firstMenu(); // It will give bit generation choices to user and
                                       // save the choice of user to given variable.
    int sub_seq_choice;  // will store the choice of subsequence given by user
    system("cls"); // clears the screen
    // will throw error if user gives any invalid choice and ask them to again input their choices.
    while(data_gen_choice>3 || data_gen_choice<1)
    {
        cout<<"\n\n\n\n\t\t\t!!!! PLEASE ENTER VALID CHOICE !!!"<<endl<<endl;
        Sleep(1000);
        system("cls");
        data_gen_choice = firstMenu();
    }
    system("cls");
    srand(time(NULL)); // random seeding done to avoid repetitive random values.
    int bit_seq_len;
    vector<int> bit_seq(bit_seq_len); // will store the generated bit stream of given bit length by user.
    system("cls");
    switch(data_gen_choice)
    {
        case 1:
            cout<<"Enter the length of bit sequence"<<endl;
            cin>>bit_seq_len;
            bit_seq = randomSeqGen(bit_seq_len); // will generate a completely random sequence
            break;
        case 2:
            cout<<"Enter the length of bit sequence"<<endl;
            cin>>bit_seq_len;
            sub_seq_choice = sub_seq_menu(); // will store the choice of sub seq given by user.
            system("cls");
            // will run the loop if user enters any invalid choice
            while(sub_seq_choice>3 || sub_seq_choice<1)
            {
                cout<<"\n\n\n\n\t\t\t!!!! PLEASE ENTER VALID CHOICE !!!"<<endl<<endl;
                Sleep(1000);
                system("cls");
                sub_seq_choice = sub_seq_menu();
            }
            system("cls");
            switch(sub_seq_choice)
            {
            case 3:
                exit(1);
                break;
            default:
                // will generate a random sequence with some fixed sub sequence.
                bit_seq = randomSeqSubSeqGen(bit_seq_len, sub_seq_choice);
                system("cls");
                break;
            }
            break;
        case 3:
            exit(1);
            break;
    }
    system("cls");
    cout<<"\n\n\t\t GENERATED BIT STREAM: ";
    bit_seq_display(bit_seq);
    bit_seq_gen_global = bit_seq;
    cout<<"\n\t\t BIT STREAM LENGTH: "<<bit_seq_len<<endl;
    bit_seq_len_global = bit_seq_len;
}

string vectorToString(vector<int> v)
{
    string str = "";
    for (int i = 0; i < v.size(); i++)
        str += to_string(v[i]);
    return str;
}

string longestPalinSubStr(string str)
{
    // Dynamic Programming Approach
    // Time Complexity: O(n^2)
    // Space Complexity: O(n^2) 2D boolean array used for memorizing.
    string res_str = ""; // it will store the resultant palindromic sub string, initially it is empty.
    int len = str.length();
    int start_p, end_p; // used to store the start and end index of LPS.
    // Corner cases.
    if (len <= 0) return res_str;
    if (len == 1) {
        res_str += str[0];
        return res_str;
    }
    // Since we want to try all the substrings of the given string, we can use a two-dimensional array
    // to store the subproblems’ results. So dp[i][j] will be ‘true’ if the substring from index ‘i’ to index ‘j’ is a palindrome.
    bool dp[len][len];
    // Base cases.
    for (int i = 0; i < len; i++) {
        dp[i][i] = true; // Every string with one element is a palindrome
    }
    int lps_max_length=1;
    for (int startIndex = len-1; startIndex >= 0; startIndex--)
    {
        for (int endIndex = startIndex+1; endIndex < len; endIndex++)
        {
            if (str[startIndex] == str[endIndex])
            {
                // if it's a two character string or if the remaining string is a palindrome too
                if (endIndex-startIndex == 1 || dp[startIndex+1][endIndex-1])
                {
                    dp[startIndex][endIndex] = true;
                    if (lps_max_length < endIndex-startIndex+1)
                    {
                        lps_max_length = endIndex-startIndex+1;
                        start_p = startIndex;
                        end_p = endIndex;
                    }
                } else dp[startIndex][endIndex] = false;
            } else dp[startIndex][endIndex] = false;
        }
    }
    for (int i = start_p; i <= end_p; i++) res_str += str[i];
    return res_str;
}

void LPS()
{
    string bit_seq_str = vectorToString(bit_seq_gen_global); // will convert the integral bits stored in  vector as concatenated single string.
    long_pal_str = longestPalinSubStr(bit_seq_str); // will find the longest palindromic string and store it in given variable.
    cout<<"\n\n\t\t GENERATED LPS: "<<long_pal_str;
    cout<<"\n\t\t LPS LENGTH: "<<long_pal_str.length()<<endl;
}

int encodingMenu()
{
    int choice;
    cout<<"\n\n\t\tCHOOSE ANY ONE ENCODING SCHEME FROM BELOW:"<<endl;
    cout<<"\n\t\t1. NRZ-L \n\t\t2. NRZ-I \n\t\t3. MACHESTER \n\t\t4. DIFF. MANCHESTER \n\t\t5. AMI \n\t\t6. EXIT"<<endl;
    cout<<"\n\t\t";
    cin>>choice;
    return choice;
}

void glut_display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void glut_initalizer(const char* window_name)
{
        glutInitWindowSize(1600, 400);
        glutInitWindowPosition(0, 60);
        glutCreateWindow(window_name);
        glutDisplayFunc(glut_display);
}

void topShow()
{
    system("cls");
    cout<<"\n\t\t BIT DATA STREAM: ";
    bit_seq_display(bit_seq_gen_global);
    cout<<"\n\t\t BIT DATA STREAM LENGTH: "<<bit_seq_len_global<<endl;
    LPS();
}

void decoder_display(vector<int> bit_seq_decoder)
{
    cout<<"\n\t\t DECODED DATA STREAM: ";
    bit_seq_display(bit_seq_decoder);
}

void graph_initializer ()
{
    int bit_len = bit_seq_len_global;
    glLineWidth(1);
    for(int i = 0 ; i <= bit_len; i++)
    {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(1,0x0FF);
        glBegin(GL_LINES);
        glColor3f( 0.5f, 0.5f, 0.5f);
        glVertex2f(i*0.05-WIDTH_SHIFT,-0.5);
        glVertex2f(i*0.05-WIDTH_SHIFT, 0.5);
        glEnd();
        glFlush();
        glDisable(GL_LINE_STIPPLE);
    }
    glBegin(GL_LINES);
    glColor3f( 1.0f, 0.0f, 0.0f);
    glVertex2f(0-WIDTH_SHIFT,0);
    glVertex2f(1.6-WIDTH_SHIFT,0);
    glVertex2f(0-WIDTH_SHIFT,0.5);
    glVertex2f(0-WIDTH_SHIFT,-0.5);
    glEnd();
    glFlush();
}

void NRZ_L(vector<int> bit_seq)
{
    graph_initializer();
    glColor3f(0, 1.0f, 0); // colors the line from one to next vertex
    int bit_seq_len = bit_seq_len_global;
    for (int i = 0; i < bit_seq_len; i++)
    {
        int flag_next = 0, flag_prev = 0;
        if (bit_seq[i] == bit_seq[i+1] && i < bit_seq_len-1) flag_next = 1; // set this to 1 if the present bit and next bit are same.
        if (bit_seq[i] == bit_seq[i-1] && i > 0) flag_prev = 1; // set this to 1 if the present bit and previous bit are same.

        if (bit_seq[i] == 0) // run this if the present bit is 0
        {
            glBegin(GL_LINE_STRIP); // start creating the line
            // if the present bit and previous bit are not same then start from x-axis and initially for first bit start from origin.
            if(!flag_prev) glVertex2f(i*0.05-WIDTH_SHIFT, 0);
            // this two lines will generate a graph in fourth quad i.e below x-axis.
            glVertex2f(i*0.05-WIDTH_SHIFT, -0.2);
            glVertex2f(i*0.05+0.05-WIDTH_SHIFT, -0.2);
            // go back to x-axis if present and next bit are not same.
            if(!flag_next) glVertex2f(i*0.05+0.05-WIDTH_SHIFT, 0);
            glEnd(); // stop creating the line.
            glFlush(); // flush the data present in buffers.
            bit_seq_encoded_global.push_back(-13); // store the -ve voltage level
        } else {
            glBegin(GL_LINE_STRIP);
            if(!flag_prev) glVertex2f(i*0.05-WIDTH_SHIFT, 0);
            // this two lines will generate a graph in first quad i.e above x-axis.
            glVertex2f(i*0.05-WIDTH_SHIFT, 0.2);
            glVertex2f(i*0.05+0.05-WIDTH_SHIFT, 0.2);
            if(!flag_next) glVertex2f(i*0.05+0.05-WIDTH_SHIFT, 0);
            glEnd();
            glFlush();
            bit_seq_encoded_global.push_back(13); // store the +ve voltage level
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

void NRZ_I_UDX (int sign, int i, int flag)
{
    // if sign is 1 draw upper half and if sign is -1 draw the lower half
    glVertex2f(i*0.05-WIDTH_SHIFT, sign*0.2);
    glVertex2f(i*0.05+0.05-WIDTH_SHIFT, sign*0.2);
    if(!flag) glVertex2f(i*0.05+0.05-WIDTH_SHIFT, 0 ); // go to x-axis if next bit is 1.
    glEnd();
    glFlush();
    bit_seq_encoded_global.push_back(sign*13);
}

void NRZ_I(vector<int> bit_seq)
{
    int bit_seq_len = bit_seq_len_global;
    graph_initializer();
    int flag = 0, flip; // here flip indicates whether the previous bit was in upper half(1) or lower half (-1)
    for(int i = 0; i < bit_seq_len; i++)
    {
        flag = 0;
        glColor3f(0, 1.0f, 0); // colors the line from one to next vertex
        if (bit_seq[i+1] == 0 && i < bit_seq_len-1) flag=1; // set the flag if next bit is 0

        if( i == 0 ) // run this if condition only for the first bit: exception case.
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f(i*0.05-WIDTH_SHIFT,0); // start from origin for the first bit always.
            if(bit_seq[i] == 1)
            {
                NRZ_I_UDX(1, i, flag);
                flip = 1; // indicates the present graph was drawn in upper half
            }
            else
            {
                NRZ_I_UDX(-1, i, flag);
                flip = -1; // indicates the present graph was drawn in lower half
            }
        } else {
            if (bit_seq[i] == 1) // if present is bit is one
            {
                glBegin(GL_LINE_STRIP);
                glVertex2f(i*0.05-WIDTH_SHIFT,0);
                if(flip == -1) // if previous graph was in lower half then drawn the present graph in upper half
                {
                    NRZ_I_UDX(1, i, flag);
                    flip = 1;
                }
                else
                {
                    NRZ_I_UDX(-1, i, flag);
                    flip = -1; // if previous graph was in upper half draw the present bit in lower half.
                }
           } else { // run this else if the present bit is 0
               glBegin(GL_LINE_STRIP);
               if (flip == -1)  // draw the present bit in lower half if the previous was in lower half.
               {
                   NRZ_I_UDX(-1, i, flag);
                   flip = -1;
                } else {
                    NRZ_I_UDX(1, i, flag);
                    flip = 1;
             }
           }
        }
     }
     //bit_seq_display(bit_seq_encoded_global);
}

void manchester (vector<int> bit_seq)
{
    graph_initializer();
    int bit_len = bit_seq_len_global;
    for (int i = 0; i < bit_len; i++)
    {
        glColor3f(0, 1.0f, 0); // colors the line from one to next vertex
        if (bit_seq[i] == bit_seq[i-1] && i > 0) // if the present and previous bit are same then flip from upper half to lower half and vice-versa.
        {
            glBegin(GL_LINES);
            glVertex2f(i*0.05-WIDTH_SHIFT, 0.2);
            glVertex2f(i*0.05-WIDTH_SHIFT, -0.2);
            glEnd();
        }
        if (bit_seq[i] == 1) // if present bit is 1 draw Z
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f(i*0.05-WIDTH_SHIFT, 0.2);
            glVertex2f(i*0.05+0.025-WIDTH_SHIFT, 0.2);
            glVertex2f(i*0.05+0.025-WIDTH_SHIFT, -0.2);
            glVertex2f(i*0.05+0.05-WIDTH_SHIFT, -0.2);
            glEnd();
            glFlush();
            bit_seq_encoded_global.push_back(13);
            bit_seq_encoded_global.push_back(-13);
        } else { // if present bit is 0 draw S.
            glBegin(GL_LINE_STRIP);
            glVertex2f(i*0.05-WIDTH_SHIFT,-0.2);
            glVertex2f(i*0.05+0.025-WIDTH_SHIFT,-0.2);
            glVertex2f(i*0.05+0.025-WIDTH_SHIFT, 0.2);
            glVertex2f(i*0.05+0.05-WIDTH_SHIFT, 0.2 );
            glEnd();
            glFlush();
            bit_seq_encoded_global.push_back(-13);
            bit_seq_encoded_global.push_back(13);
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

void diffMan_UDX (int sign, int i)
{
    glBegin(GL_LINE_STRIP);
    glColor3f(0, 1.0f, 0); // colors the line from one to next vertex
    // draws Z if sign is 1 and draws S if the sign  is -1
    glVertex2f(i*0.05-WIDTH_SHIFT, sign*0.2);
    glVertex2f(i*0.05+0.025-WIDTH_SHIFT, sign*0.2);
    glVertex2f(i*0.05+0.025-WIDTH_SHIFT, sign*-0.2);
    glVertex2f(i*0.05+0.05-WIDTH_SHIFT, sign*-0.2 );
    glEnd();
    glFlush();
    bit_seq_encoded_global.push_back(sign*13);
    bit_seq_encoded_global.push_back(sign*-13);
}

void diffManchester (vector<int> bit_seq)
{
    graph_initializer();
    int bit_len = bit_seq_len_global;
    for (int i = 0; i < bit_len; i++)
    {
        if (i == 0) // for first bit draw Z for 1 and S for 0
        {
            if (bit_seq[i] == 1) diffMan_UDX(1, i);
            else diffMan_UDX(-1, i);
        } else { // for all the bits except for the first bit run this
            if (bit_seq[i] == 1)
            {
                // if last vertex of graph was in positive half draw Z or continue from there if present bit is 1
                if(bit_seq_encoded_global[2*i-1] == 13) diffMan_UDX(1, i);
                // if last vertex of graph was in negative half draw S or continue from there if present bit is 1
                else diffMan_UDX(-1, i);
            } else {
                glBegin(GL_LINES);
                // if present bit is 0 first flip from lower to upper half and vice-versa.
                glColor3f( 0, 1.0f, 0);
                glVertex2f(i*0.05-WIDTH_SHIFT,0.2);
                glVertex2f(i*0.05-WIDTH_SHIFT,-0.2);
                glEnd();
                if(bit_seq_encoded_global[2*i-1] == -13) diffMan_UDX(1, i);
                else diffMan_UDX(-1, i);
            }
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

void scramb_drawer (int sign, int i)
{
    glBegin(GL_LINE_STRIP);
    glColor3f( 0, 1.0f, 0);
    glVertex2f(i*0.05-WIDTH_SHIFT,0);
    glVertex2f(i*0.05-WIDTH_SHIFT, sign*0.3);
    glVertex2f(i*0.05+0.05-WIDTH_SHIFT, sign*0.3);
    glVertex2f(i*0.05+0.05-WIDTH_SHIFT,0);
    glEnd();
    glFlush();
    sign = !sign?100:sign*13;
    bit_seq_encoded_global.push_back(sign);
}

void AMI (vector<int> bit_seq)
{
    graph_initializer();
    int bit_seq_len = bit_seq_len_global, flag = 0; // here flag is used to draw alternate graphs i.e flag 0 draws graph in upper half and flag 1 draws graph in lower half.
    for (int i = 0; i < bit_seq_len; i++)
    {
        if (bit_seq[i] == 0) scramb_drawer(0, i); // if the bit is 0 just draw a flat line on x-axis
        else {
            if (!flag)
            {
                scramb_drawer(1, i);
                flag = 1;
            } else {
                scramb_drawer(-1, i);
                flag = 0;
            }
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

void HDB3 (vector<int> bit_seq)
{
    graph_initializer();
    int bit_seq_len = bit_seq_len_global;
    int i = 0, one_c = -1, one_c_t = 0; // one_c will keep the count of 1's and one_c_t keep the count of pulses
    while ( i < bit_seq_len) // run while loop for all the bits
    {
        if (bit_seq[i] == 1) // if the bit is one run the if condition
        {
            one_c_t++;
            one_c++;
            if (!(one_c & 1)) scramb_drawer(1, i); // draw the graph in upper half for all odd count of 1 i.e 1, 3, 5.....
            else scramb_drawer(-1, i); // draw the graph in lower half for all even count of 1 i.e. 0, 2, 4.....
            i++; // move to next bit
        } else if (bit_seq[i] == 0) { // if the the bit is 0 run this
            int zero_c = 0; // keeps the count of 0
            int pointer_one = i, pointer_two = pointer_one + 3; // keep one pointer on 1st 0 and next pointer on 4th 0.
            if (pointer_two < bit_seq_len) // if second pointer is within range then run this if condition
            {
                while (pointer_one <= pointer_two) // will check for four consecutive 0's
                {
                    if (bit_seq[pointer_one] == 0) zero_c++; // if the bit is zero increase the count
                    else break;
                    pointer_one++;
                }
            }
            if (zero_c == 4) // will run the if condition if there are four consecutive 0's
            {
                if (!(one_c_t & 1)) // if count of pulses is even or we can say if previous bit graph was drawn in lower half run this if condition.
                {
                    one_c++; // increase this by one so the next pulse will be in opposite half.
                    int pointer_o = 0, pointer_t = i;
                    while (i <= pointer_t+3) // changing the 4 0's to B00V
                    {
                        if (pointer_o == 0 || pointer_o == 3) // draw the pulse for B & V.
                        {
                            if (!(one_c & 1)) scramb_drawer(1, i);
                            else scramb_drawer(-1, i);
                        } else scramb_drawer(0, i); // draw a line on x-axis for normal 0
                        pointer_o++;
                        i++;
                    }
                    one_c_t += 2; // increase the pulse count by two in case of B00V.
                } else if (one_c_t & 1) { // run this if count of pulse is odd.
                    int pointer_o = 0, pointer_t = i;
                    while (i <= pointer_t+3) // manage four 0's to 000V
                    {
                        if (pointer_o == 3) // for last 0 i.e. replace it by V
                        {
                            if ((one_c & 1)) scramb_drawer(-1, i);
                            else scramb_drawer(1, i);;
                        } else scramb_drawer(0, i);
                        pointer_o++;
                        i++;
                    }
                    one_c_t++; // increase the count of pulse by 1
                }
            } else { // just draw a flat line on x-axis if bit is 0 and there are no consecutive four 0's
                scramb_drawer(0, i);
                i++;
            }
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

void B8ZS (vector<int> bit_seq)
{
    graph_initializer();
    int bit_seq_len = bit_seq_len_global;
    int one_c = 1, i = 0;
    while (i < bit_seq_len)
    {
        if (bit_seq[i] == 1)  // if the bit is one run the if condition
        {
            one_c++;
            if(!(one_c & 1)) scramb_drawer(1, i); // draw the graph in upper half for all odd count of 1 i.e 1, 3, 5.....
            else scramb_drawer(-1, i); // draw the graph in lower half for all even count of 1 i.e. 0, 2, 4.....
            i++; // move to next bit
        } else if (bit_seq[i] == 0) { // if the the bit is 0 run this
            int zero_c = 0, pointer_one = i; // keeps the count of 0
            int pointer_two = pointer_one + 7; // keep one pointer_one 1st 0 and next pointer on 8th 0.

            if (pointer_two < bit_seq_len-2) // if second pointer is within range then run this if condition
            {
                while ( pointer_one <= pointer_two ) // will check for eight consecutive 0's
                {
                    if ( bit_seq[pointer_one] == 0) zero_c++; // if the bit is zero increase the count
                    else break;
                    pointer_one++;
                }
            }
            if (zero_c == 8) // will run the if condition if there are eight consecutive 0's
            {
                int pointer_o = 0, pointer_t = i;
                while (i <= pointer_t+3) // this will manage first 4 0's 0000 to 000V
                {
                    if (pointer_o == 3) // convert 4th 0 to V.
                    {
                        if(one_c & 1) scramb_drawer(-1, i);
                        else scramb_drawer(1, i);
                    } else scramb_drawer(0, i);
                    i++;
                    pointer_o++;
                }
                one_c++;
                pointer_o = 0;
                pointer_t = i;
                while ( i <= pointer_t + 3) // manages next 4 0's to B0VB.
                {
                    if (pointer_o == 0)
                    {
                        if (!(one_c & 1)) scramb_drawer(1, i);
                        else scramb_drawer(-1, i);
                    } else {
                        if (pointer_o == 2)
                        {
                            if ( !(one_c & 1)) scramb_drawer(1, i);
                            else scramb_drawer(-1, i);
                            i++;
                            pointer_o++;
                        }
                        if (pointer_o == 3)
                        {
                            if (!(one_c & 1)) scramb_drawer(-1, i);
                            else scramb_drawer(1, i);
                            one_c++;
                        } else scramb_drawer(0, i);
                    }
                    i++;
                    pointer_o++;
                }
            } else { // will draw the flat line on x-axis if the bit is 0 and there is no consecutive 8 0's
                scramb_drawer(0,i);
                i++;
            }
        }
    }
    //bit_seq_display(bit_seq_encoded_global);
}

vector<int> NRZ_LDec(vector<int> bit_seq_encoded_global)
{
    vector<int> bit_seq_decoded;
    for(int i=0;i< bit_seq_encoded_global.size();i++)
    {
    if( bit_seq_encoded_global[i]<0)
        bit_seq_decoded.push_back(0);
    else
        bit_seq_decoded.push_back(1);
    }
    return bit_seq_decoded;
}

vector<int> NRZ_IDec(vector<int> bit_seq_encoded_global)
{
   vector<int> bit_seq_decoded;
    for(int i=0;i< bit_seq_encoded_global.size();i++)
    {
    if(i==0)
    {
            if( bit_seq_encoded_global[i]>0)
              bit_seq_decoded.push_back(1);
            else
              bit_seq_decoded.push_back(0);
    }
    else if(( bit_seq_encoded_global[i]>0&& bit_seq_encoded_global[i-1]<0)||( bit_seq_encoded_global[i]<0&& bit_seq_encoded_global[i-1]>0))
        bit_seq_decoded.push_back(1);
    else bit_seq_decoded.push_back(0);
   }
    return bit_seq_decoded;
}

vector<int> manchesterDec(vector<int> bit_seq_encoded_global)
{
    vector<int> bit_seq_decoded;
    for(int i=0;i< bit_seq_encoded_global.size();i+=2)
        if( bit_seq_encoded_global[i]>0) bit_seq_decoded.push_back(1);
        else bit_seq_decoded.push_back(0);
    return bit_seq_decoded;
}

vector<int> diffManchesterDec(vector<int> bit_seq_encoded_global)
{
    vector<int> bit_seq_decoded;
    for(int i=0;i< bit_seq_encoded_global.size();i+=2)
    {
        if(i==0)
        {
            if( bit_seq_encoded_global[i]>0) bit_seq_decoded.push_back(1);
            else bit_seq_decoded.push_back(0);
        } else if(( bit_seq_encoded_global[i]>0&& bit_seq_encoded_global[i-1]<0)||( bit_seq_encoded_global[i]<0&& bit_seq_encoded_global[i-1]>0))
            bit_seq_decoded.push_back(0);
        else bit_seq_decoded.push_back(1);
    }
    return bit_seq_decoded;
}

vector<int> AMIDec(vector<int> bit_seq_encoded_global)
{
    vector<int> bit_seq_decoded;
    for(int i=0;i< bit_seq_encoded_global.size();i++)
        if( bit_seq_encoded_global[i]==100) bit_seq_decoded.push_back(0);
        else bit_seq_decoded.push_back(1);
    return bit_seq_decoded;
}

vector<int> B8ZSDec(vector<int> bit_seq_encoded_global)
{
    vector<int> bit_seq_decoded;
    int i=0;
    int prev=-13;
    int n=bit_seq_encoded_global.size();
    while(i<n)
    {
        if( bit_seq_encoded_global[i]==100)
        {
            bit_seq_decoded.push_back(0);
            i++;
        }
        else if( bit_seq_encoded_global[i]==-prev)
        {
            bit_seq_decoded.push_back(1);
            prev=-prev;
            i++;
        }
        else
        {
            //Violation Case: change the VB00VB to 000000
            int j;
            for(j=i;j<i+5;j++)
            {
                bit_seq_decoded.push_back(0);
            }
            i=i+5;
        }
    }
    return bit_seq_decoded;
}

vector<int> HDB3Dec(vector<int> bit_seq_encoded_global)
{
    int i=0;
    int n=bit_seq_encoded_global.size();
    vector<int> bit_seq_decoded(n);
    int prev=-13;
    while(i<n)
    {
        if( bit_seq_encoded_global[i]==100)
        {
            bit_seq_decoded[i]=0;
            i++;
        }
        else if( bit_seq_encoded_global[i]==-prev)
        {
            bit_seq_decoded[i]=1;
            prev=-prev;
            i++;
        }
        else
        {
            //Violation Case: change the 000V and B00V to 0000
            bit_seq_decoded[i]=0;
            bit_seq_decoded[i-3]=0;
            i++;
        }
    }
    return bit_seq_decoded;
}

void scramblingMenu()
{
    int tech_choice;
    system("cls");
    cout<<"\n\t\t you have selected AMI line encoding method"<<endl;
    cout<<"\n\t\t PLEASE CHOOSE SCRAMBLING TECHNIQUE:"<<endl;
    cout<<"\t\t1. B8ZS \n\t\t2. HDB3\n\n\t\t";
    cin>>tech_choice;
    topShow();
    vector<int> bit_seq_decoder(bit_seq_encoded_global.size());
    switch(tech_choice)
    {
    case 1:
        glut_initalizer("B8ZS");
        B8ZS(bit_seq_gen_global);
        bit_seq_decoder=B8ZSDec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    case 2:
        glut_initalizer("HDB3");
        HDB3(bit_seq_gen_global);
        bit_seq_decoder=HDB3Dec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    default:
        system("cls");
        cout<<"\n\n\n\t\t\t !!! ENTER VALID CHOICE !!!"<<endl;
        Sleep(1000);
        scramblingMenu();
        break;
    }
}

void amiChoice()
{
    int scram_choice;
    system("cls");
    cout<<"\n\t\t you have selected AMI line encoding method"<<endl;
    cout<<"\n\t\t DO YOU WANT TO USE SCRAMBLER:"<<endl;
    cout<<"\t\t1. YES \n\t\t2. NO\n\n\t\t";
    cin>>scram_choice;
    vector<int> bit_seq_decoder;
    switch(scram_choice)
    {
    case 1:
        topShow();
        scramblingMenu();
        break;
    case 2:
        glut_initalizer("AMI");
        topShow();
        AMI(bit_seq_gen_global);
        bit_seq_decoder=AMIDec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    default:
        system("cls");
        cout<<"\n\n\n\t\t\t !!! ENTER VALID CHOICE !!!"<<endl;
        Sleep(1000);
        amiChoice();
        break;
    }
}

void encodingScheme()
{
    int encoding_choice = encodingMenu(); // it will store the encoding choice given by user
    while (encoding_choice>6 || encoding_choice<1) // it will throw error and ask user to again input choice if he gives any invalid choice.
    {
        cout<<"\n\t\t !!! ENTER VALID CHOICE !!!"<<endl;
        encoding_choice = encodingMenu();
    }
    topShow(); // it just display the original bit stream and its length on top of display screen
    vector<int> bit_seq_decoder; // it will be used to store the decoded output obtained from voltage levels.
    switch(encoding_choice)
    {
    case 1:
        glut_initalizer("NRZ-L"); // it initializes the name of graph-window to "NRZ-L".
        NRZ_L(bit_seq_gen_global); // consists programming logic of NRZ-L and code to print graph and takes but stream as I/P params.
        bit_seq_decoder=NRZ_LDec(bit_seq_encoded_global); // calling decoder function, takes voltage level as input and gives back the bit stream back as the output.
        decoder_display(bit_seq_decoder); // displays the decoded output.
        break;
    case 2:
        glut_initalizer("NRZ-I");
        NRZ_I(bit_seq_gen_global);
        bit_seq_decoder=NRZ_IDec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    case 3:
        glut_initalizer("MANCHESTER");
        manchester(bit_seq_gen_global);
        bit_seq_decoder=manchesterDec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    case 4:
        glut_initalizer("DIFFERENTIAL MANCHESTER");
        diffManchester(bit_seq_gen_global);
        bit_seq_decoder=diffManchesterDec(bit_seq_encoded_global);
        decoder_display(bit_seq_decoder);
        break;
    case 5:
        amiChoice(); // gives the user ami choices with or without scrambling.
        break;
    case 6:
        exit(1);
        break;
    }
}

void glut_shapedefine(GLsizei width, GLsizei height)
{
    if (height == 0) height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();
    if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }
}

void displayScreen()
{
    bitSeqChoice(); // will give all the bit generation choices
    LPS(); // will find and display longest palindromic substring in data/bit stream
    encodingScheme(); // will give all the choices for encoding the bit stream
    glutReshapeFunc(glut_shapedefine); // will set the dimensions of display screen of the graph.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // will clear the color buffers-RGBA format
                                          // and set the color of graph display screen to black with no opacity.
    glutMainLoop(); // glutMainLoop is the main function the keeps calling and calling the
                    // displaying functions and which also keeps your window actually opened.
}

int main(int argc, char** argv)
{
    // glutInit will initialize the GLUT library
    // and negotiate a session with the window system
    glutInit(&argc, argv);
    // Creates main display screen
    displayScreen();
    return 0;
}
