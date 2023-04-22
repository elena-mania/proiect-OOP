//
// Created by elena on 4/22/2023.
//
#include <iostream>
#include <cstring>
using namespace std;

class String{
    char *str;
    int size;
public:
    friend class Song;
    String() : str(nullptr), size(0) {}
    explicit String(const char sir[]){
        if(sir){
            size=strlen(sir);
            this->str=new char[size+1];
            strcpy(this->str,sir);
        }
    }

    String(const String& s){
        this->size=s.size;
        if(s.str){
            this->str=new char[size+1];
            strcpy(str, s.str);
        }
        else this->str=nullptr;
    }

    const char* getString() {return str;}

    void setString(const char* s) {
        size=strlen(s);
        delete[] str;
        str=new char[size+1];
        strcpy(str,s);
    }

    String& operator=(const String& sir){
        if(this!=&sir){
            delete[] str;
            size=sir.size;
            str=new char[size + 1];
            strcpy(str, sir.str);
        }
        return *this;
    }

    bool operator==(const String& sir){
        if(this->size!=sir.size)
            return false;
        for (int i=0;i<size;++i)
            if (this->str[i]!=sir.str[i])
                return false;
        return true;
    }

    friend ostream& operator << (ostream& out, const String& string);
    ~String(){
        if(str!=nullptr)
            delete[] str;
    }
};

ostream& operator << (ostream& out, const String& string){
    out<<string.str;
    return out;
}

class Song{ ///Song - This class represents a single song, with attributes such as the title, artist, album, and duration.
    String title, artist, album;
    int min, sec;
public:
    Song() {
        title.setString("");
        artist.setString("");
        album.setString("");
        min=0;
        sec=0;
    }
    Song(const char* title, const char* artist, const char* album, int min=0, int sec=0){
        this->title.setString(title);
        this->artist.setString(artist);
        this->album.setString(album);
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
    String getTitle() {return this->title;}
    String getArtist() {return this->artist;}
    String getAlbum() {return this->album;}

    int getMin() const {return this->min;}
    int getSec() const {return this->sec;}

};

class Playlist{ ///Playlist - This class represents a playlist, which is a collection of songs. It has attributes such as the name of the playlist and a vector of songs.
    String playlistName;
    int numberOfSongs;
    Song* songs;
public:
    Playlist() : playlistName("Default"), numberOfSongs(0), songs(nullptr){}

    explicit Playlist(const String& name) : playlistName(name), numberOfSongs(0), songs(nullptr){}

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

    void playlistLength(){
        int len=0;
        float lenSec=0;
        for(int i=0;i<numberOfSongs;++i){
            len+=songs[i].getMin();
            lenSec+=float(songs[i].getSec());
        }
        len+=int(lenSec/60);
        while(lenSec>59){
            lenSec-=60;
        }
        cout<<len<<" "<<lenSec<<'\n';
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

    void setPlaylistName(String& name){this->playlistName=name;}

    ~Playlist(){
        if(songs!= nullptr)
            delete[] songs;
    }
};

class User{
    String username,email,password;
public:
    User()=default;
    User(const String& username, const String& email, const String& password){
        this->username=username;
        this->email=email;
        this->password=password;
    }
    void modifyPassword(String oldPass, String& newPass){ ///this method allows the user to change his password
        if(oldPass==password){
            password=newPass;
            cout<<"Password has been successfully changed!\n";
        }
        else
            cout<<"Incorrect password!\n";
    }
    String getPass(){return this->password;}
    void setUsername(String& name){this->username=name;}
    void setEmail(String& mail){this->email=mail;}

};

int main() {
    ///cateva melodii
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

    Song song11,song12(song9),song13;
    song11=song13=song7;
    cout<<song11.getArtist()<<" "<<song13.getTitle()<<"\n";

    ///un user
    String username("popescu_andrei"), mail("popescu.andrei@yahoo.com"), pass("1234567890"),newmail("pop.and@gmail.com");
    User popescu(username,mail,pass);

    ///modificarea parolei contului in urma introducerii parolei vechi (ca metoda de securitate)
    String newpass("0987654321");
    popescu.modifyPassword(String("parola_gresita"),newpass);
    popescu.modifyPassword(String("1234567890"),newpass);
    cout<<"My new password is: "<<popescu.getPass()<<endl;

    popescu.setUsername(mail);
    popescu.setEmail(newmail);

    ///un exemplu de playlist
    String name("Primul meu playlist");
    Playlist myPlaylist1(name);

    myPlaylist1.addSong(song2);
    myPlaylist1.addSong(song5);
    myPlaylist1.addSong(song8);
    myPlaylist1.addSong(song10);
    myPlaylist1.printPlaylist();

    myPlaylist1.setPlaylistName(username);
    myPlaylist1.removeSong(song2);
    myPlaylist1.printPlaylist();

    myPlaylist1.playlistLength();

    return 0;
}