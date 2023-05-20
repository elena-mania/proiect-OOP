#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <stdexcept>
#include <limits>
#include <algorithm>

using namespace std;

class InvalidInputException : public runtime_error {
public:
    explicit InvalidInputException(const string& message)
            : runtime_error(message) {}
};
class InvalidDuration : public runtime_error {
public:
    explicit InvalidDuration(const string& message) : runtime_error(message) {}
};
bool isValidEmail(const string& email) {
    regex regexPattern(R"(\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}\b)");
    return regex_match(email, regexPattern);
}

class Song{ ///Song - This class represents a single song, with attributes such as the title, artist, album, and duration.
    string title, artist, album;
    int min, sec;
public:
    Song() {
        title=""; artist=""; album=""; min=0; sec=0;
    }
    Song(const char* title, const char* artist, const char* album, int min=0, int sec=0){
        this->title=title;
        this->artist=artist;
        this->album=album;
        this->min=min;
        this->sec=sec;
    }
    Song(const Song& other){
        title=other.title;
        artist=other.artist;
        album=other.album;
        min=other.min;
        sec=other.sec;
    }
    Song& operator=(const Song& other){
        if(this==&other)
            return *this;
        title=other.title;
        artist=other.artist;
        album=other.album;
        min=other.min;
        sec=other.sec;
        return *this;
    }
    string getTitle() {return this->title;}
    string getArtist() {return this->artist;}
    string getAlbum() {return this->album;}

    friend istream& operator>>(istream& input, Song& other) {
        string stitle, sartist, salbum;
        int smin, ssec;
        cout<<"Enter the title: ";
        input>>stitle;
        cout<<"Enter the artist: ";
        input>>sartist;
        cout<<"Enter the album: ";
        input>>salbum;
        while (true) {
            try {
                cout<<"Enter the length in minutes: ";
                input>>smin;
                if (input.fail()) {
                    input.clear();
                    input.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw InvalidInputException("The input is not valid, please try again later!\n");
                } else {
                    break;
                }
            } catch (InvalidInputException& except) {
                cout << except.what();
            }
        }
        while (true) {
            try {
                cout<<"Enter the length in seconds: ";
                input>>ssec;
                if (input.fail()) {
                    input.clear();
                    input.ignore(numeric_limits<streamsize>::max(), '\n');
                    throw InvalidInputException("The input is not valid, please try again later!\n");
                } else {
                    if(ssec >= 60){
                        input.clear();
                        input.ignore(numeric_limits<streamsize>::max(), '\n');
                        throw InvalidDuration("The duration is not valid, pleaes try again later!\n");
                    }
                    else {
                        break;
                    }
                }
            } catch (InvalidInputException& except) {
                cout << except.what();
            }
            catch (InvalidDuration& except) {
                cout << except.what();
            }
        }
        other.title=stitle;
        other.artist=sartist;
        other.album=salbum;
        other.min=smin;
        other.sec=ssec;
        return input;
    }

};
class Playlist{ ///Playlist - This class represents a playlist, which is a collection of songs. It has attributes such as the name of the playlist and a vector of songs.
    string playlistName;
    int numberOfSongs;
    Song* songs;
public:
    Playlist() : playlistName("Default"), numberOfSongs(0), songs(nullptr){}

    explicit Playlist(string& name) : playlistName(name), numberOfSongs(0), songs(nullptr){}

    void addSong(const Song& newSong){
        Song* newSongs=new Song[numberOfSongs+1];
        for(int i=0;i<numberOfSongs; i++)
            newSongs[i]=songs[i];
        newSongs[numberOfSongs] = newSong;
        numberOfSongs++;
        delete[] songs;
        songs=newSongs;
    }

    void removeSong(Song& oldSong){
        int index=-1,i;
        for(i=0;i<numberOfSongs;++i)
            if(songs[i].getTitle()==oldSong.getTitle()){
                index=i; break;
            }
        if(index!=-1) {
            Song *newSongs = new Song[numberOfSongs - 1];
            for(i=0;i<index;++i)
                newSongs[i]=songs[i];
            for(i=index+1;i<numberOfSongs;++i)
                newSongs[i - 1] = songs[i];
            numberOfSongs--;
            if(numberOfSongs == 0){
                delete[] songs;
                songs=nullptr;
            }
            else{
                delete[] songs;
                songs=newSongs;
            }
        }
    }

    void printPlaylist(){
        cout<<"Playlist Name: "<<playlistName<<endl;
        cout<<"Number of Songs: "<<numberOfSongs<<endl;
        for (int i = 0; i < numberOfSongs; i++) {
            cout<<"Song #"<<i+1<< ":"<<endl;
            cout<<"\tTitle: "<<songs[i].getTitle()<<endl;
            cout<<"\tArtist: "<<songs[i].getArtist()<<endl;
            cout<<"\tAlbum: "<<songs[i].getAlbum()<<endl;
        }
    }

    string getPlaylistName() const{return playlistName;}

    void setPlaylistNumber(int nr){this->numberOfSongs=nr;}

    ~Playlist(){
        if(songs!= nullptr)
            delete[] songs;
    }
};

class User{
public:
    User()=default;
    virtual string getUsername() const=0;
    virtual string getPassword() const=0;
    virtual void printWelcome() const=0;
    virtual void modifyPassword(string oldpass, string newpass)=0;
    virtual void printUserInfo()=0;
    virtual ~User()=default;
};

class Artist: virtual public User{
    string username,email,password,genre;
public:

    Artist()=default;
    Artist(string user, string mail, string pass, string gen): username(user), email(mail), password(pass), genre(gen){}
    string getUsername() const override {return username;}
    string getPassword() const override{return password;}
    void printWelcome() const override {
        cout << "Welcome, " << getUsername() << "!" << endl;
    }
    void modifyPassword(string oldPass, string newPass) override{ ///this method allows the user to change his password
        if(oldPass==password){
            password=newPass;
            cout<<"Password has been successfully changed!\n";
        }
        else
            cout<<"Incorrect password!\n";
    }
    void printUserInfo() override{
        cout<<"Artist Name: "<<username<<endl;
        cout<<"Business email: "<<email<<endl;
        cout<<"Genre: "<<genre<<endl;
    }

    ~Artist() override = default;
};

class Consumer: virtual public User{
    string username,email,password;
    bool monthlyFee{};
public:
    Consumer()=default;
    Consumer(string user, string mail, string pass, bool ok=false): username(user), email(mail), password(pass), monthlyFee(ok){}
    string getUsername() const override {return username;}
    string getPassword() const override {return password;}
    void printWelcome() const override {
        cout << "Welcome, " << getUsername() << "!" << endl;
    }
    void modifyPassword(string oldPass, string newPass) override{
        if(oldPass==password){
            password=newPass;
            cout<<"Password has been successfully changed!\n";
        }
        else
            cout<<"Incorrect password!\n";
    }
    void printUserInfo() override{
        cout<<"Your username: "<<username<<endl;
        cout<<"Your email: "<<email<<endl;
    }
    // New member function to check if the monthly fee is paid
    bool hasPaidMonthlyFee() const { return monthlyFee;}

    // Member function to update the monthly fee payment status
    void setMonthlyFeePaymentStatus(bool paid) {monthlyFee=paid;}

    ~Consumer() override =default;
};

class MusicPlayer {
protected:
    bool isPlaying;
public:
    MusicPlayer() : isPlaying(false) {}
    virtual ~MusicPlayer() = default;
    virtual void play() {
        isPlaying = true;
        cout << "Music is playing." <<endl;
    }
    void pause() {
        isPlaying = false;
        cout << "Music is paused."<<endl;
    }
    void skip() const{
        if (isPlaying)
            cout<< "Skipping to the next song."<<endl;
        else
            cout << "Cannot skip while music is paused." <<endl;
    }
};

class MediaPlayer : virtual public MusicPlayer {
    int volume;
public:
    explicit MediaPlayer(int initialVolume) : volume(initialVolume) {}

    void setVolume(int newVolume) {
        volume = newVolume;
        cout<< "Volume set to " << volume <<endl;
    }
};

class StreamingPlayer : virtual public MusicPlayer{
    string streamingService;
public:
    explicit StreamingPlayer(const string& service) : streamingService(service) {}
    string getName() const {return streamingService;}
    void displayService() const {
        cout << "Streaming service: " << streamingService<<endl;
    }
};

class SmartMusicPlayer : public MediaPlayer, public StreamingPlayer {
    static int playerCount;
public:
    SmartMusicPlayer(int initialVolume, const string& service)
            : MediaPlayer(initialVolume), StreamingPlayer(service), MusicPlayer() {
        playerCount++;
    }
    static int getPlayerCount() {return playerCount;}

    void play() override {
        cout<<StreamingPlayer::getName() << " play function."<<endl;
        MusicPlayer::play();
    }
    ~SmartMusicPlayer() override {playerCount--;}
};
int SmartMusicPlayer::playerCount = 0;


class MenuLogIn{
    vector <User*> users;
    int logged_user;
    vector <Playlist*> playlists;
    vector <SmartMusicPlayer*> players;
public:
    MenuLogIn() {
        logged_user=-1;
        string usernameDefault="artist",emailDefault="email",passwordDefault="pass",genreDefault="rock";
        users.emplace_back(new Artist(usernameDefault,emailDefault,passwordDefault,genreDefault));
    }
    void registerUser() {
        string name, password, email;
        bool validEmail = false;

        while (!validEmail) {
            try {
                cout << "Enter your username: ";
                cin >> name;
                cout << "Enter your email: ";
                cin >> email;
                if (!isValidEmail(email)) {
                    throw invalid_argument("The email is invalid!");
                }
                validEmail = true;
            } catch (const exception &e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        cout << "Enter your password: ";
        cin >> password;
        User* user = new Consumer(name, email, password);
        users.emplace_back(user);
        cout << " ---------------------------------- " <<endl;
        cout << "Your account has been successfully created!" <<endl;
        cout << " ---------------------------------- " <<endl;
    }

    void logIn() {
        bool ok=false;
        string name, password;
        cout<<"Enter the username: ";
        cin>>name;
        cout<<"Enter the password: ";
        cin>>password;
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getUsername() == name && users[i]->getPassword() == password) {
                logged_user = i;
                ok= true;
                break;
            }
        }
        if(ok){
            cout << "===================================" << endl;
            cout << users[logged_user]->getUsername()<< ", Welcome to Musicplayer!"<<endl;
            cout << "===================================" << endl;
        }
        else
            cout<<"Incorrect password or username!"<<endl;
    }

    void logOut() {
        logged_user = -1;
        cout << "You have been successfully logged out!" << endl;
    }
    void printUsers() {
        for (int i = 1; i < users.size(); i++) {
            cout << i << ". " << users[i]->getUsername() << endl;
            cout<<"-----------------------------------"<<endl;
        }
    }

    void printUserInfo(){
        users[logged_user]->printUserInfo();
        cout<<endl;
        cout<<"Do you wish to change your password? (y/n): ";
        char c;
        cin>>c;
        if(c=='y'){
            string oldpass,newpass;
            cout<<"Enter your old password: ";
            cin>>oldpass;
            cout<<"Enter your new password: ";
            cin>>newpass;
            users[logged_user]->modifyPassword(oldpass,newpass);
        }
    }

    void deleteAccount() {
        string userToRemove;
        cout<<"Enter the username of your account: ";
        cin>>userToRemove;
        users.erase(std::remove_if(users.begin(), users.end(),[userToRemove](User* user) {return user->getUsername() == userToRemove; }), users.end());
        cout<<"Your account has been successfully deleted!"<<endl;
    }

    void addAlbum() {
        if (logged_user!=-1) {
            string name;
            cout << "Enter the name of the Album:" << endl;
            cin >> name;
            auto* myPlaylist = new Playlist(name);
            int i = 0;
            char condition;
            cout << "Add song #" << i << " :\n";
            Song s;
            cin >> s;
            myPlaylist->addSong(s);
            i++;
            cout << "Do you wish to add more? (y/n)\n";
            cin >> condition;
            while (true) {
                switch (condition) {
                    case 'y': {
                        cout << "Add song #" << i << " :\n";
                        cin >> s;
                        myPlaylist->addSong(s);
                        i++;
                        cout << "Do you wish to add more? (y/n)\n";
                        cin >> condition;
                        break;
                    }
                    case 'n': {
                        cout << "Your album has been successfully created!\n";
                        myPlaylist->setPlaylistNumber(i);
                        playlists.push_back(myPlaylist);
                        return;
                    }
                    default: {
                        cout << "Invalid command!\n";
                        break;
                    }
                }
            }
        }
    }

    void getPlaylists(){
        for(int i=0;i<playlists.size();i++){
            cout<<i+1<<". "<<playlists[i]->getPlaylistName()<<endl;
        }
    }

    void viewPlaylist() {
        if(logged_user!=-1){
            if (playlists.empty()) {
                cout << "There are no playlists/albums on the database yet!\n";
                return;
            }
            cout << "Your playlists are:\n";
            getPlaylists();
            cout<<"Do you wish to sort the playlists by name? (y/n): ";
            char condition;
            cin>>condition;
            if(condition=='y'){
                sort(playlists.begin(),playlists.end(),[](Playlist* a,Playlist* b){return a->getPlaylistName()<b->getPlaylistName();});
                cout<<"The albums have been successfully sorted:\n";
                getPlaylists();
            }else{
                cout<<"The albums have not been sorted!\n";
                return;
            }
            cout << "Do you wish to view a specific playlist? (y/n)\n";
            cin >> condition;
            if (condition == 'y') {
                cout << "Enter the index of the playlist you wish to view: ";
                int i;
                cin >> i;
                if (i > playlists.size() || i < 0) {
                    cout << "Invalid index!\n";
                    return;
                }else{
                    playlists[i-1]->printPlaylist();
                    cout<<endl;
                    cout<<"Do you wish to listen to this playlist? (y/n): ";
                    cin>>condition;
                    if(condition=='y'){
                        navigateSmartMusicPlayer();
                    }
                    else return;
                }
            } else {
                return;
            }
        }
    }

    void mothlyFeeStatusUpdate(){
        for (auto user : users) {
            if (auto consumer = dynamic_cast<Consumer*>(user)) {
                if(!consumer->hasPaidMonthlyFee()){
                    cout<<"You have not paid the monthly fee yet!\n";
                    cout<<"Do you wish to pay it now? (y/n): ";
                    char c;
                    cin>>c;
                    if(c=='y'){
                        consumer->setMonthlyFeePaymentStatus(true);
                        cout<<"The monthly fee has been successfully paid!\n";
                    }else{
                        cout<<"You will not be able to use the smart music player until you pay the monthly fee!\n";
                    }
                }else{
                    cout<<"You have already paid the monthly fee!\n";
                }
            }
        }
    };

    void createSmartMusicPlayer(){
        string name;
        int vol;
        cout<<"Enter the name of the smart music player: ";
        cin>>name;
        cout<<"Enter the initial volume: ";
        cin>>vol;
        auto* smp = new SmartMusicPlayer(vol,name);
        players.push_back(smp);
        cout<<"The smart music player has been successfully created!\n";
    }

    void navigateSmartMusicPlayer(){
        if(players.empty()){
            cout<<"There are no smart music players on the database yet!\n";
            return;
        }
        cout<<"The smart music players are:\n";
        for(int i=0;i<players.size();i++){
            cout<<i+1<<". "<<players[i]->getName()<<endl;
        }
        int i;
        cout<<"Enter the index of the smart music player you wish to navigate: ";
        cin>>i;
        if(i>players.size()||i<0){
            cout<<"Invalid index!\n";
            return;
        }
        else if(!players.empty()){
            players[i-1]->displayService();
            printMeniuMusicPlayer();
            int option;
            cout<<"Enter the option: ";
            cin>>option;
            while(true){
                switch(option){
                    case 1:{
                        players[i-1]->play();
                        break;
                    }
                    case 2:{
                        players[i-1]->pause();
                        break;
                    }
                    case 3:{
                        players[i-1]->skip();
                        break;
                    }
                    case 4:{
                        cout<<"Enter the volume: ";
                        int vol;
                        cin>>vol;
                        if(vol>=0&&vol<=100)
                            players[i-1]->setVolume(vol);
                        else {cout<<"Invalid volume!\n"; return;}
                        break;
                    }
                    case 5:{
                        return;
                    }
                    default:{
                        cout<<"Invalid option!\n";
                        break;
                    }
                }
                printMeniuMusicPlayer();
                cout<<"Enter the option: ";
                cin>>option;
            }
        }
    }

    User* get_user(int i) {return users[i];}

    int get_logged_user() const {return logged_user;}

    static void printMenu1() {
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. View Number of smart music players" << endl;
        cout << "4. View Users"<<endl;
        cout << "5. Exit" << endl;
    }

    static void printMenu2() {
        cout << "----------------------------------" << endl;
        cout << "What do you wish to do?" << endl;
        cout << "----------------------------------" << endl;
        cout << "1. View Playlists" << endl;
        cout << "2. Subscription status" << endl;
        cout << "3. View account data"<<endl;
        cout << "4. Delete account" << endl;
        cout << "5. Logout" << endl;
    }

    static void printMenu2Artist() {
        cout<<"----------------------------------"<<endl;
        cout<< "What do you wish to do?" << endl;
        cout<< "----------------------------------" << endl;
        cout<<"1. View Albums" << endl;
        cout << "2. View account data"<<endl;
        cout<<"3. Add Album"<<endl;
        cout<<"4. Create Smart music player"<<endl;
        cout << "5. Logout" << endl;
    }

    static void printMeniuMusicPlayer(){
        cout<<"----------------------------------"<<endl;
        cout<< "What do you wish to do?" << endl;
        cout<< "----------------------------------" << endl;
        cout<<"1. Play"<<endl;
        cout<<"2. Pause"<<endl;
        cout<<"3. Skip"<<endl;
        cout<<"4. Change volume"<<endl;
        cout<<"5. Finish"<<endl;
    };

    ~MenuLogIn() {
        for (User *user: users)
            delete user;
        users.clear();
        for (Playlist* playlist : playlists)
            delete playlist;
        playlists.clear();
        for (SmartMusicPlayer* player : players)
            delete player;
        players.clear();
    }
};


int main() {
    ///baza de date de melodii
    Song song1("One","Metallica","...And Justice For All",7,27);
    Song song2("Song 2", "Blur", "Blur: the best of",2,0);
    Song song3("Don't speak", "No doubt", "Tragic Kingdom", 4, 23);
    Song song4("Black hole sun", "Soundgarden", "Superunknown",5,19);
    Song song5("Je veux", "Zaz", "Zaz",3,36);
    Song song6("Sunetul mai tare", "Vita de vie", "Exxtra",3,40);
    Song song7("Seven Nation Army", "The White Stripes", "Elephant",3 ,52);
    Song song8("Time","Pink Floyd", "The Dark Side Of The Moon",7,3);
    Song song9("Enjoy the silence", "Depeche mode", "Violator",4 ,15);
    Song song10("Even Flow","Pearl Jam", "Ten",4,53);

    ///exemplu de upcasting
    Artist artist1("Gwen Stefani", "gwen@stefani.com","password", "ska punk");
    Consumer consumer1("Elena", "elena@mania.ro","pass");
    User *ptrArtist1 = &artist1;
    User *ptrConsumer1= &consumer1;
    ptrArtist1->printUserInfo();
    ptrConsumer1->printUserInfo();

    MenuLogIn meniu;
    int choice1;
    while(true){
        if (meniu.get_logged_user()==-1){
            MenuLogIn::printMenu1();
            cin>>choice1;
            switch(choice1){
                case 1:{
                    meniu.logIn();
                    break;}
                case 2:{
                    meniu.registerUser();
                    break;}
                case 3:{
                    cout << "Number of smart music players: " << SmartMusicPlayer::getPlayerCount()<<endl;
                    break;
                }
                case 4:{
                    meniu.printUsers();
                    break;}
                case 5:{
                    cout<<"Goodbye!"<<endl;
                    return 0;}
                default:{
                    cout << "Invalid choice!" << endl;
                    return 0;}
            }
        } else {
            auto *art = dynamic_cast<Artist *>(meniu.get_user(meniu.get_logged_user()));
            if (art) {
                MenuLogIn::printMenu2Artist();
                cin >> choice1;
                switch (choice1) {
                    case 1:
                        meniu.viewPlaylist();
                        break;
                    case 2:
                        meniu.printUserInfo();
                        break;
                    case 3: {
                        meniu.addAlbum();
                        break;
                    }
                    case 4:{
                        meniu.createSmartMusicPlayer();
                        break;}
                    case 5: {
                        meniu.logOut();
                        break;
                    }
                    default: {
                        cout << "Invalid choice!" << endl;
                        return 0;
                    }
                }
            } else {
                auto *cons = dynamic_cast<Consumer *>(meniu.get_user(meniu.get_logged_user()));
                cons->printWelcome();
                MenuLogIn::printMenu2();
                cin >> choice1;
                switch (choice1) {
                    case 1:
                        meniu.viewPlaylist();
                        break;
                    case 2:
                        meniu.mothlyFeeStatusUpdate();
                        break;
                    case 3:
                        meniu.printUserInfo();
                        break;
                    case 4:
                        meniu.deleteAccount();
                        meniu.logOut();
                        break;
                    case 5:
                        meniu.logOut();
                        break;
                    default: {
                        cout << "Invalid choice!" << endl;
                        return 0;
                    }
                }
            }
        }
    }
    return 0;
}