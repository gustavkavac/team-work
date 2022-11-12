#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <vector>

#define FILE_NAME "the-file.txt"

using namespace std;

class Item {
public:
    char itemName[20];
    double itemPrice, itemQuantity, itemsSold;

    void readItem() {
        cout << endl << "Informacija par produktu:" << endl;
        cout << "Nosaukums: ";
        cin.getline(itemName, 20);
        cout << "Cena: ";
        cin >> itemPrice;
        cout << "Pieejamais skaits: ";
        cin >> itemQuantity;
        cout << "Pardotais skaits: ";
        cin >> itemsSold;
    }
    void displayItem() {
        cout << left << setw(15) << itemName
             << left << setw(15) << itemPrice
             << left << setw(15) << itemQuantity
             << left << setw(15) << itemsSold << endl;
    }
    char *getItemName() { return itemName; }
};

int topThreeWorstSelling(string fileName);
int writeToFile(Item item, string filename);
int readFromFile(string filename);
int sellProduct(string fileName, string productName);
int searchProduct(string fileName, string productName);
void choiceInfo();

int main() {
    Item item;
    string input;
    int menuItem;
    cout << "Loading..." << '\n' << endl;
    cout << "*********************NOLIKTAVA*********************" << endl;

    while (true) {
        choiceInfo();
        cout << endl << "Kada ir Tava izvele?: ";
        cin >> menuItem;
        switch(menuItem) {
        case 0:
            return 0;
        case 1:
            cin.ignore();
            writeToFile(item, FILE_NAME);
            break;
        case 2:
            readFromFile(FILE_NAME);
            break;
        case 3:
            cout << "Kuru produktu pardod?" << endl;
            cin >> input;
            sellProduct(FILE_NAME, input);
            break;
        case 4:
            cout << "Meklet prouktu: ";
            cin >> input;
            searchProduct(FILE_NAME, input);
            break;
        case 5:
            topThreeWorstSelling(FILE_NAME);
            break;
        default:
            cout << "\nKluda! Nepareiza izvelne!" << endl;
            return 0;
        }
    }
    return 0;
}

int topThreeWorstSelling(string fileName){
    vector<Item> fileData;
    vector<Item> fileDataExeptWorst;
    vector<Item> fileDataExeptTwoWorst;
    Item product;
    Item worstProduct;
    Item secondWorstProduct;
    Item thirdWorstProduct;
    int productInteger = 0;
    int productQuantityInteger = 0;
    int theWorst = 0;
    int worstIter = 0;
    bool go = true;
    
    ifstream fileObj_in(fileName, ios::in | ios::binary); //ieseivo vektora all data info 
    if (!fileObj_in.is_open()){
    cout << "Kluda atverot failu" << endl;
    return -1;
    }
    while (!fileObj_in.eof()){
        fileObj_in.read((char*)&product, sizeof(product));
        fileData.push_back(product);
        productInteger++;
    }
    fileData.pop_back();
    productInteger--;
    productQuantityInteger = productInteger;
    fileObj_in.close();
    
    // galvena logjika
    theWorst = fileData[0].itemsSold; //atrod vismazako
    for (int i = 0; i <= fileData.size(); i++){
        if (fileData[i].itemsSold <= theWorst){
            worstProduct = fileData[i];
            theWorst = fileData[i].itemsSold;         
        }
    }
    for (int i = 0; i < fileData.size(); i++){ //ieliek jauna vektora bez worstProduct
        if (strcmp(fileData[i].itemName, worstProduct.itemName)){
            fileDataExeptWorst.push_back(fileData[i]);
        }
    }
    theWorst = fileDataExeptWorst[0].itemsSold; //talak tapati logjika
    for (int i = 0; i < fileDataExeptWorst.size(); i++){
        if (fileDataExeptWorst[i].itemsSold <= theWorst){
            secondWorstProduct = fileDataExeptWorst[i];  
            theWorst = fileDataExeptWorst[i].itemsSold;
        }
    }
    for (int i = 0; i < productQuantityInteger; i++){
        if (strcmp(fileDataExeptWorst[i].itemName, secondWorstProduct.itemName)){
            if (strcmp(fileData[i].itemName, worstProduct.itemName)) {
                fileDataExeptTwoWorst.push_back(fileData[i]);
            }
        }
    }
    theWorst = fileDataExeptTwoWorst[0].itemsSold;
    for (int i = 0; i < fileDataExeptTwoWorst.size(); i++){
        if (fileDataExeptTwoWorst[i].itemsSold <= theWorst){
            thirdWorstProduct = fileDataExeptTwoWorst[i];
            theWorst = fileDataExeptTwoWorst[i].itemsSold;
        }
    }


    // cout << '\n' << '\n'; //tiek pie visiem vektora datiem
    // for (int i = 0; i < productQuantityInteger; i++){
    //     cout << fileDataExeptTwoWorst[i].itemName << '\t' << fileDataExeptTwoWorst[i].itemsSold << endl;
    // }
    
    worstProduct.displayItem();
    secondWorstProduct.displayItem();
    thirdWorstProduct.displayItem();
    return 0;
}

int sellProduct(string fileName, string productName){
    Item product;
    Item soldProduct;
    bool go = true;
    vector<Item> fileData;
    vector<Item> soldFileData;

    double quantity;
    double sold;
    int productInteger = 0;
    int soldProductInteger = 0;
    int productQuantityInteger = 0;

    ifstream fileObj_in(fileName, ios::in | ios::binary); //ieseivo vektora all data info 
    if (!fileObj_in.is_open()){
    cout << "Kluda atverot failu" << endl;
    return -1;
    }
    while (!fileObj_in.eof()){
        fileObj_in.read((char*)&product, sizeof(product));
        fileData.push_back(product);
        productInteger++;
    }
    fileData.pop_back();
    productInteger--;
    productQuantityInteger = productInteger;
    fileObj_in.close();

    for (int i = 0; i < productQuantityInteger; i++) {
        if (!strcmp(productName.c_str(), fileData[i].itemName)) {
            strcpy(soldProduct.itemName, product.itemName);
            soldProduct.itemPrice = product.itemPrice;
            soldProduct.itemQuantity = product.itemQuantity - (double)1;
            soldProduct.itemsSold = product.itemsSold + (double)1;   
            soldFileData.push_back(soldProduct);
        } else {
            soldFileData.push_back(fileData[i]);
        }
    }
    
    remove(fileName.c_str());
    fstream file;
    file.open(fileName, ios::out|ios::binary|ios::app);
    if (!file){
        cout << "Kluda rakstot faila" << endl;
        return -1;
    }
    for (int i = 0; i < soldFileData.size(); i++) {
    file.write((char*)&soldFileData[i],sizeof(soldFileData[i]));
    }
    file.close();

    return 0;
}

int searchProduct(string fileName, string productName){
    Item product;
    ifstream file;

    file.open(fileName, ios::in);
    if (!file){
        cout << "Kluda atverot failu" << endl;
        return -1;
    }
    if (file.read((char*)&product, sizeof(product))){ //loops iet cauri produktiem lidz sakrit nosaukumi
        while (!file.eof()){
            if (product.getItemName() == productName){
                cout << endl
                     << left << setw(15) << "Nosaukums"
                     << left << setw(15) << "Cena"
                     << left << setw(15) << "Pieejams"
                     << left << setw(15) << "Pardots" << endl;
                product.displayItem();
                break;
            }
            file.read((char*)&product, sizeof(product));
        }
    }
    else {
        cout << "Kluda nolasot no faila" << endl;
        return -1;
    }
    file.close();
    cout << endl;
    return 0;
}

int readFromFile(string filename) {
    Item item;
    fstream file;
    file.open(filename,ios::in|ios::binary);
    if (!file){
        cout << "Kluda atverot failu" << endl;
        return -1;
    }
    if (file.read((char*)&item,sizeof(item))) {
        cout << endl
             << left << setw(15) << "Nosaukums"
             << left << setw(15) << "Cena"
             << left << setw(15) << "Pieejams"
             << left << setw(15) << "Pardots" << endl;
        while(!file.eof()) {
            item.displayItem();
            file.read((char*)&item,sizeof(item));
        }
    }
    else {
        cout << "Kluda nolasot no faila" << endl;
        return -1;
    }
    file.close();
    return 0;
}

int writeToFile(Item item, string filename) {
    item.readItem();
    fstream file;
    file.open(filename, ios::out|ios::binary|ios::app);
    if (!file){
        cout << "Kluda rakstot faila" << endl;
        return -1;
    }
    file.write((char*)&item,sizeof(item));
    file.close();
    cout << "Saglabats faila" << endl;
    return 0;
}

void choiceInfo() {
    cout << '\n';
    cout << "---------------IZVELNE--------------" << '\n'; 
    cout << "0: Beigt darbibu" << '\n';
    cout << "1: Pievienot produktu" << '\n';
    cout << "2: Paradit visus produktus" << '\n';
    cout << "3: Pardot produktu" << '\n';
    cout << "4: Meklet produktu" << '\n';
    cout << "5: TOP 3 vismazak pardotie produkti" << '\n';
    cout << "------------------------------------";
}