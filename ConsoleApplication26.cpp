#include<iostream>
#include<Windows.h>
#include"dirent.h"
#include<sys/stat.h>
#include<fstream>
#include<cstring>
#include<string>
#include<algorithm>
#include<conio.h>
#include<stdio.h>
#include<cstring>
#include <Mmsystem.h>
#include <mciapi.h>
//#include<string>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

using namespace std;

class Media_player
{
private:
	string manufacturer_name;
	string model_num;

public:

	Media_player(string model_name = "Chord Blu", string modelnum = "MkII") :manufacturer_name(model_name), model_num(modelnum)
	{}
	string get_manufacturer_name()
	{
		return manufacturer_name;
	}
	string get_model_num()
	{
		return model_num;
	}
};

class audio_player
{
protected:

	string addr;
public:
	audio_player() : addr("0") {}
	audio_player(string a) :addr(a) {}

	void play()									//It will play the song 
	{
		cout << "\nPlaying" + addr << "\n";
		string command = "play " + addr;
		mciSendStringA(command.c_str(), NULL, 0, NULL);
	}
	void pause()								//It will pause the song 
	{
		cout << "\n" + addr + " is paused\n";
		string command = "pause " + addr;
		mciSendStringA(command.c_str(), NULL, 0, NULL);
	}

	void resume()								//It will resume the song 
	{
		cout << "\n" + addr + " is resumed\n";
		string command = "resume " + addr;
		mciSendStringA(command.c_str(), NULL, 0, NULL);
	}

	void stop()									//It will stop the song and you cannot resume the song
	{
		cout << "\n" + addr + " is stopped\n";
		string command = "stop " + addr;
		mciSendStringA(command.c_str(), NULL, 0, NULL);
	}

	void replay()								//It will replay the song
	{
		stop();
		play();
	}

};

class storage_devices :public audio_player
{
private:
	bool check = false;
	int choice = 0;
	string address;
	string name;
	string playlist_name;
	string* arr;
	fstream obj;
	int count = 0;

public:
	storage_devices() :audio_player()
	{
		address = "0";
		name = "0";
		arr = NULL;
	}
	storage_devices(string address1, string name1, string x) :audio_player(x), address(address1), name(name1)
	{
		arr = NULL;
	}



	void mount()							//It will mount the device
	{
		if (check == true)
		{
			cout << "\nUnmount the device\n";
		}
		else if (check == false)
		{
		check = true;
		cout << "\nDevice is mounted\n";
		readfile();
		}
	}

	void readfile()							//It will read choosen device
	{
		count = 0;
		cout << "\nEnter 1 for CD\nEnter 2 for USB\n";
		int choice;
		cout << "Enter here : ";
		cin >> choice;
		cout << "\n";
		if (choice == 1)
		{
			DIR* pdir = NULL;

			pdir = opendir(".\\devices\\DiskDrive");  /// "." will refer to the current directory

			struct dirent* pent = NULL;

			if (pdir == NULL) // if pdir wasn't initialised correctly

			{
				cout << "\nERROR! pdir could not be initialised correctly";
				exit(3);

			}

			while (pent = readdir(pdir)) // while there is still something in the directory to list

			{

				if (pent == NULL) // if pent has not been initialised correctly

				{
					cout << "\nERROR! pent could not be initialised correctly";
					exit(3);
				}

				string name = pent->d_name;
				if (name.length() > 4)
				{
					address = ".\\devices\\DiskDrive\\" + name;
					count++;
				}
			}
			int index = 0;
			arr = new string[count];

			pdir = NULL;

			pdir = opendir(".\\devices\\DiskDrive");  /// "." will refer to the current directory

			pent = NULL;

			while (pent = readdir(pdir)) // while there is still something in the directory to list

			{

				if (pent == NULL) // if pent has not been initialised correctly

				{
					cout << "\nERROR! pent could not be initialised correctly";
					exit(3);
				}

				string name = pent->d_name;
				if (name.length() > 4)
				{
					address = ".\\devices\\DiskDrive\\" + name;
					arr[index] = address;								//Saving data of directory in dynamic array
					index++;
				}


			}


		}

		else if (choice == 2)
		{
			DIR* pdir = NULL;

			pdir = opendir(".\\devices\\USB");  /// "." will refer to the current directory  

			struct dirent* pent = NULL;


			if (pdir == NULL)

			{
				cout << "\nERROR! pdir could not be initialised correctly";
				exit(3);

			}

			while (pent = readdir(pdir))

			{

				if (pent == NULL)

				{
					cout << "\nERROR! pent could not be initialised correctly";
					exit(3);
				}



				string n = pent->d_name;
				if (n.length() > 4) {
					name = ".\\devices\\USB\\" + n;
					count++;							//Reading number of entries present in directory
				}
			}
			int index = 0;
			arr = new string[count];

			pdir = NULL;

			pdir = opendir(".\\devices\\USB");  /// "." will refer to the current directory  

			pent = NULL;

			while (pent = readdir(pdir)) // while there is still something in the directory to list

			{

				if (pent == NULL) // if pent has not been initialised correctly

				{
					cout << "\nERROR! pent could not be initialised correctly";
					exit(3);
				}

				string name = pent->d_name;
				if (name.length() > 4)
				{
					address = ".\\devices\\USB\\" + name;
					arr[index] = address;					//Saving data of directory in dynamic array
					index++;
				}


			}
		}

	}

	void Unmount()						//It will unmount the mounted device and it will delete the array that contain data of mounted directory
	{
		if (check == true)
		{
			stop();
			cout << "\nDevices is unmounted\n";
			check = false;
			address = "0";
			addr = "0";
			count = 0;
			delete[]arr;
			arr = NULL;					//Preventing the creation of dangling pointer

		}
		else
		{
			cout << "\nNothing is mounted\n";
		}
	}

	void show_list()					//It will show list of all songs present in directory
	{
		cout << "\n";
		for (int x = 0; x < count; x++)
		{
			cout << arr[x] << "\n";
		}
	}


	void select_audio()					//When user will enter song number it will play the song according to that number
	{
		show_list();
		cout << "\nEnter number of song you want to play : ";
		cin >> choice;
		cout << "\n";
		choice = (choice - 1);
		addr = arr[choice];
		play();
	}

	void jump_to()						//It will jump to desired song and stop already playing function
	{
		stop();
		select_audio();
	}

	void shuffle_playlist()
	{
		stop();
		random_shuffle(&arr[0], &arr[count]);
	}
	void previous()
	{
		if (arr[choice] == arr[0])
		{
			cout << "\nCan't play previous song, You are already at first song\n";
		}
		else if (arr[choice] != arr[0])
		{
			stop();
			addr = arr[(choice - 1)];
			choice = choice - 1;
			play();
		}
	}
	void next()
	{
		if (arr[choice] == arr[count - 1])
		{
			cout << "\nCan't play next song, You are already at last song\n";
		}
		else if (arr[choice] != arr[count - 1])
		{
			stop();
			addr = arr[(choice + 1)];
			choice = choice + 1;
			play();
		}
	}
	void delete_song()
	{
		string temp;
		choice = 0;
		show_list();
		cout << "Enter that song that you want to delete : ";
		cin >> choice;
		ofstream writefile;
		ifstream readfile;
		
		readfile.open(playlist_name.c_str());
		temp = arr[choice - 1];
		readfile.close();
		writefile.open(playlist_name.c_str());
		int counter = 0;
		while (counter <count)
		{
			
			if (arr[counter] != temp)
			{
				writefile << arr[counter] << "\n";
			}
			else if(arr[counter]==temp)
			{
				cout << "Song is deleted";;
			
			}
			counter++;
			
		}
		
		writefile.close();

		remove(playlist_name.c_str());

	}
	void save_playlist()
	{
		if (check == true)
		{
			cout << "Enter name of playlist : ";
			cin.ignore();
			getline(cin, playlist_name);
			playlist_name += ".txt";
			obj.open(playlist_name.c_str(), ios::out | ios::app);
			show_list();
			cout << "Select the audio that you want to save to file : ";

			while (true)
			{
				cin >> choice;
				if (choice <= count && choice >= 0)
				{
					string x1 = arr[choice - 1];

					obj << x1 << " \n";
				} 
				else
				{
					break;
				}
			}

			obj.close();
		}
		else if (check == false)
		{
			cout << "\nPlease mount the device\n";
		}
		
	}

	void load_playlist()
	{
		if (check == false)
		{
			check = true;
			int index = 0;
			count = 0;
			string temp;
			cout << "Enter name of playlist : ";
			cin.ignore();
			getline(cin, playlist_name);
			playlist_name += ".txt";
			{
				obj.open(playlist_name.c_str(), ios::in);
				getline(obj, temp);
				
				while (!obj.eof())
				{
					cout << temp << "\n";
					count++;
					getline(obj, temp);
				}
			
				obj.close();
				
				cout << count;
			}

			arr = new string[count];
			{
				obj.open(playlist_name.c_str(), ios::in);
				
				getline(obj, temp);
				while (!obj.eof())
				{
					arr[index] = temp;
					index++;
					getline(obj, temp);
				}
				
			}
		}
		else if (check == true)
		{
			cout << "\nUnmount the device\n";
		}
		

	}

	void add_song()
	{
		if (check == true)
		{
			cout << "Enter name of playlist in which you want to add songs : ";
			cin.ignore();
			getline(cin, playlist_name);
			playlist_name += ".txt";
			obj.open(playlist_name.c_str(), ios::out | ios::app);
			show_list();
			cout << "Select the audio that you want to add to playlist : ";

			while (true)
			{
				cin >> choice;
				if (choice <= count && choice >= 0)
				{
					string x1 = arr[choice - 1];

					obj << x1 << "\n";
				}
				else
				{
					break;
				}
			}
			obj.close();
		}
		else if(check == false)
		{
			cout << "\nPlease mount the device\n";
		}
		

	}
	void search_song()
	{
		string temp_name;
		string temp_addr;
		int choose=0;
		bool check_song = false;
		cout << "\nIf song was from CD folder enter 1\nIf song was from USB enter 2\n";
		cout << "Enter you choice here : ";
		cin >> choose;
		cout << "\n\nEnter song that you want to search : ";
		cin >> temp_name;
		
		
		if (choose==1)
		{
			temp_addr = ".\\devices\\DiskDrive\\" + temp_name + ".mp3 ";
			for (int x = 0; x < count; x++)
			{
				if (temp_addr == arr[x])
				{
					check_song = true;
					cout << "\nSong is available\n";
					break;
				}
			}
			if (check_song == false)
			{
				cout << "\nSong is not available in CD\n";
			}
		}
		else if (choose == 2)
		{
			int counter = count;
			temp_addr = ".\\devices\\USB\\" + temp_name + ".mp3";
			for (int x = 0; x < counter; x++)
			{
				if (temp_addr == arr[x])
				{
					cout << "\nSong is available\n";
					break;
				}
			}
		}
		
	}
};



int main()
{
	int choice = 0;

	Media_player a;
	storage_devices b;
	cout << "\n\n\t\t\t\t" << a.get_manufacturer_name() << " " << a.get_model_num() << "\n\n\n";


	while (true)								//While loop that will run infinetly
	{

		cout << "Enter 1 to mount\nEnter 2 to show list of songs\nEnter 3 to play a sound\nEnter 4 to pause a song\nEnter 5 to resume\nEnter 6 to stop\nEnter 7 to replay\nEnter 8 to jump to desired song\nEnter 9 to unmount\nEnter 10 to shuffle the array\nEnter 11 to play previous song\nEnter 12 to play next song\nEnter 13 to save playlist\nEnter 14 to load a playlist\nEnter 15 to add songs in playlist\nEnter 16 to delete song\nEnter 17 to search song\n";
		cout << "Here enter the choice : ";
		cin >> choice;

		//system("CLS") will clear the previous screen and Sleep(2000) will pause the program for given time and time is given in milisecond

		switch (choice)
		{
		case 1:
			Sleep(250);
			system("CLS");
			b.mount();
			Sleep(1500);
			system("CLS");
			break;
		case 2:
			Sleep(250);
			system("CLS");
			b.show_list();
			Sleep(5000);
			system("CLS");
			break;
		case 3:
			Sleep(250);
			system("CLS");
			b.select_audio();
			Sleep(1500);
			system("CLS");
			break;
		case 4:
			Sleep(250);
			system("CLS");
			b.pause();
			Sleep(1500);
			system("CLS");
			break;
		case 5:
			Sleep(250);
			system("CLS");
			b.resume();
			Sleep(1500);
			system("CLS");
			break;
		case 6:
			Sleep(250);
			system("CLS");
			b.stop();
			Sleep(1500);
			system("CLS");
			break;
		case 7:
			Sleep(250);
			system("CLS");
			b.replay();
			Sleep(1500);
			system("CLS");
			break;

		case 8:
			Sleep(250);
			system("CLS");
			b.jump_to();
			Sleep(1500);
			system("CLS");
			break;
		case 9:
			Sleep(250);
			system("CLS");
			b.Unmount();
			Sleep(1500);
			system("CLS");
			break;
		case 10:
			Sleep(250);
			system("CLS");
			b.shuffle_playlist();
			Sleep(1500);
			system("CLS");
			break;
		case 11:
			Sleep(250);
			system("CLS");
			b.previous();
			Sleep(1500);
			system("CLS");
			break;
		case 12:
			Sleep(250);
			system("CLS");
			b.next();
			Sleep(1500);
			system("CLS");
			break;
		case 13:
			Sleep(250);
			system("CLS");
			b.save_playlist();
			Sleep(1500);
			system("CLS");
			break;
		case 14:
			Sleep(250);
			system("CLS");
			b.load_playlist();
			Sleep(1500);
			system("CLS");
			break;
		case 15:
			Sleep(250);
			system("CLS");
			b.add_song();
			Sleep(1500);
			system("CLS");
			break;
		case 16:
			Sleep(250);
			system("CLS");
			b.delete_song();
			Sleep(1500);
			system("CLS");
			break;
		case 17:
			Sleep(250);
			system("CLS");
			b.search_song();
			_getch();
			system("CLS");
			break;
		}

	}


	return 0;

}
