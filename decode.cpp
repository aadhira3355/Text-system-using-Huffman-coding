#include "decode.hpp"
#include "headers.hpp"

codes1 decodes[256];
codes1 short_map[256];
int short_map_count = 0;

char ans[2000] = "";
int ans_len = 0;

int total_count = 0;

void decode() {
    decode_menu();
}

void decode_menu() {
    int ch;
    while (true) {
        cout << "\n\n===========>\tDecode Menu\t<===========\n" << endl;
        cout << "0. Go back to main menu" << endl;
        cout << "1. Enter filename to decode" << endl;
        cout << "\nEnter your choice: ";
        cin >> ch;
        if (!ch) {
            cout << "\nGoing back to main menu." << endl;
            break;
        }
        switch (ch) {
            case 1:
                decode_file();
                break;
            default:
                cout << "\nEnter right choice (0-1)." << endl;
        }
    }
}

void print_decode_codes() {
    cout << "\n\nCorresponding Huffman code for characters are: " << endl;
    for (int i = 0; i < 256; i++) {
        if (short_map[i].character != '\0') {
            cout << short_map[i].character << " - " << short_map[i].code << endl;
            if (i%10==0)
            	sleep(1);
        }
    }
    cout << "\n";
}

void generate_short_map() {
    for (int i = 0; i < 256; i++) {
        if (decodes[i].character != '\0') {
            short_map[short_map_count] = decodes[i];
            short_map_count += 1;
        }
    }
    cout << "\n";
}

void decode_file() {
    short_map_count = 0;
    total_count = 0;

    cout << "\n\n=====>\tDecode File\t<=====\n" << endl;
    char filename[120];
    cout << "Enter file name of code map (dat): ";
    cin >> filename;

    int len = strlen(filename);
    if (len < 3 || filename[len - 1] != 't' || filename[len - 2] != 'a' || filename[len - 3] != 'd') {
        cout << "\nEnter valid filename (dat)." << endl;
        return;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        cout << "\nFile does not exist, please try again." << endl;
        return;
    }
    char buffer;
    int c = -1;
    int flag = 0;
    char temp[20] = "";
    fread(&total_count, sizeof(total_count), 1, fp);

    cout << "\nTotal no of bits to be read: " << total_count << endl;
    sleep(1);
    while (fread(&buffer, sizeof(char), 1, fp)) {
        if (buffer == '1' || buffer == '0') {
            flag = 1;
            if (strlen(temp) < sizeof(temp) - 1) {
                strncat(temp, buffer == '1' ? "1" : "0", 1);
            }
        } else {
            if (flag == 1 && c != -1) {
                strcpy(decodes[c].code, temp);
                flag = 0;
            }
            temp[0] = '\0';
            c = (unsigned char)buffer;
            decodes[c].character = buffer;
        }
    }
    if (flag == 1 && c != -1) {
        strcpy(decodes[c].code, temp);
    }
    fclose(fp);

    generate_short_map();

    cout << "Total no of unique characters = " << short_map_count << endl;
    sleep(1);
    print_decode_codes();

    cout << "\nEnter file to decode (dat): ";
    char todecode[120];
    cin >> todecode;
    len = strlen(todecode);
    if (len < 3 || todecode[len - 1] != 't' || todecode[len - 2] != 'a' || todecode[len - 3] != 'd') {
        cout << "\nEnter valid filename (dat)\n";
        return;
    }
    cout << "\nDecoded file name: uncompressed.txt\n";
	sleep(1);
    decode_file_2(todecode);
    cout << "\nDecoded file contents: " << endl;
    sleep(1);
    for (int i = 0; i < total_count; i++) {
        cout << ans[i];
    }
    FILE *rp = fopen("uncompressed.txt", "w");
    cout << "\n\nUncompressed file: " << endl;
    sleep(1);
    char buffer2[200] = "";
    for (int i = 0; i < total_count; i++) {
        buffer = ans[i];
        if (strlen(buffer2) < sizeof(buffer2) - 1) {
            strncat(buffer2, buffer == '1' ? "1" : "0", 1);
        }
        for (int k = 0; k < short_map_count; k++) {
            if (strcmp(buffer2, short_map[k].code) == 0) {
                fprintf(rp, "%c", short_map[k].character);
                buffer2[0] = '\0';
                cout << short_map[k].character;
            }
        }
    }

    fclose(rp);
}

void decode_file_2(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        cout << "Error opening file: " << filename << endl;
        return;
    }
    char buffer;
    while (fread(&buffer, sizeof(char), 1, fp)) {
        char_to_binary(buffer);
    }
    fclose(fp);
}

// function to convert char to binary
void char_to_binary(char ch) {
    int i = 7;
    while (i >= 0) {
        if (ans_len < sizeof(ans) - 1) {
            strcat(ans, (ch & (1 << i)) ? "1" : "0");
            ans_len++;
        }
        i--;
    }
}

