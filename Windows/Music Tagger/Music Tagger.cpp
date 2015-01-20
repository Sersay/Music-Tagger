// Music Tagger.cpp : Defines the entry point for the console application.
//

//Step 1. List all songs and respective tags for id3v1
//Step 2. Access updated list and update tags
//Step 3. Rename song titles based on artist and song title tags

#include "stdafx.h"

CWinApp tagger;

using namespace std;

class MP3
{
public:
	MP3(string newpathname)
	{
		pathname = newpathname;

	}
	void setTitle(string newtitle)
	{
		title = newtitle;
	}
	void setArtist(string newartist)
	{
		artist = newartist;
	}
	void setAlbum(string newalbum)
	{
		album = newalbum;
	}
	string getTitle()
	{
		return title;
	}
	string getArtist()
	{
		return artist;
	}
	string getAlbum()
	{
		return album;
	}
	string getPathname()
	{
		return pathname;
	}
private:
	string title;
	string artist;
	string album;
	string pathname;
};

vector <MP3 *> getMusicList(CAtlString musicfolder);
string allowCommas(string input);
string checkInput(string input);
int DisplayContinueMessageBox();
void createMyMusicList(vector <MP3 *> mp3list);

int main()
{
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) 
		return 1;

	TCHAR currentdir[200];
	GetCurrentDirectory(200, currentdir);
	int choice = 0;
	while (choice != 4)
	{
		SetCurrentDirectory(currentdir);
		cout << "Welcome to Music Tagger v1.00\n" << endl;
		cout << "Please Select an option\n1.Create a list of your library\n2.Update your library based on thie list from #1\n3.Change song file names to reflect their artist and title\n4.Exit Music Tagger\n5.Open Readme with instructions." << endl;
		cout << "Choice:";

		cin >> choice;

		if (choice == 4)
		{
			cout << "Thank you for using Music Tagger!" << endl;
			system("timeout 3");
			return 0;
		} else if (choice == 5)
		{
			system("start notepad \"readme.txt\"");
		}
		else
		{
			CFolderPickerDialog folderselect;

			if (folderselect.DoModal() == IDOK)
				AfxMessageBox(folderselect.GetFolderPath());

			CAtlString folderpathvs = folderselect.GetFolderPath().GetString();
			folderpathvs.AppendFormat(_T("\\"));
			SetCurrentDirectory(folderpathvs);

			vector <MP3*> mp3list = getMusicList(folderpathvs);

			switch (choice)
			{
			case 1:
			{
					  createMyMusicList(mp3list);
					  break;
			}
			case 2:
			{
					  int exit = DisplayContinueMessageBox();
					  if (exit == 7)
					  {
						  break;
					  }

					  ifstream musiclist;
					  vector <MP3*>::iterator it;

					  musiclist.open("MyMusicList.csv", fstream::in);

					  char trash[1000];
					  musiclist.getline(trash, 1000);

					  for (it = mp3list.begin(); it != mp3list.end(); it++)
					  {
						  char artist[33] = "", title[33] = "", album[33] = "";
						  if (musiclist.peek() == ',')
						  {
							  musiclist.ignore();
						  }
						  else
						  {
							  musiclist.get(artist, 33, ',');
							  musiclist.ignore();
						  }
						  if (musiclist.peek() == ',')
						  {
							  musiclist.ignore();
						  }
						  else
						  {
							  musiclist.get(title, 33, ',');
							  musiclist.ignore();
						  }
						  if (musiclist.peek() == ',')
						  {
							  musiclist.ignore();
						  }
						  else
						  {
							  musiclist.get(album, 33, ',');
							  musiclist.ignore();
						  }
						  musiclist.getline(trash, 1000);
						  (*it)->setArtist(checkInput(artist));
						  (*it)->setTitle(checkInput(title));
						  (*it)->setAlbum(checkInput(album));
					  }
					  musiclist.close();

					  fstream mp3filestream;

					  for (it = mp3list.begin(); it != mp3list.end(); it++)
					  {
						  mp3filestream.open((*it)->getPathname(),fstream::in);
						  mp3filestream.seekg(-128, ios_base::end);
						  char tag[4];
						  mp3filestream.get(tag, 4);
						  mp3filestream.close();
						  
						  if (strcmp("TAG", tag) == 0)
						  {
							  cout << "here1" << endl;
							  mp3filestream.open((*it)->getPathname(), fstream::out | fstream::in);
							  mp3filestream.seekg(-125,ios_base::end);

							  mp3filestream << (*it)->getTitle();
							  for (unsigned int i = 0; i < (30 - (*it)->getTitle().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream << (*it)->getArtist();
							  for (unsigned int i = 0; i < (30 - (*it)->getArtist().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream << (*it)->getAlbum();
							  for (unsigned int i = 0; i < (30 - (*it)->getAlbum().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  for (int i = 0; i < 35; i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream.close();
						  }
						  else
						  {
							  cout << "here2" << endl;
							  mp3filestream.open((*it)->getPathname(), fstream::app);
							  mp3filestream.write("TAG", 3);
							  mp3filestream << (*it)->getTitle();
							  for (unsigned int i = 0; i < (30 - (*it)->getTitle().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream << (*it)->getArtist();
							  for (unsigned int i = 0; i < (30 - (*it)->getArtist().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream << (*it)->getAlbum();
							  for (unsigned int i = 0; i < (30 - (*it)->getAlbum().length()); i++)
							  {
								  mp3filestream.put(' ');
							  }
							  for (int i = 0; i < 35; i++)
							  {
								  mp3filestream.put(' ');
							  }
							  mp3filestream.close();
						  }
					  }
					  break;
			}
			case 3:
			{
					  int exit = DisplayContinueMessageBox();
					  if (exit == 7)
					  {
						  break;
					  }
					  ifstream musiclist;
					  vector <MP3*>::iterator it;
					  char artist[33], title[33];

					  musiclist.open("MyMusicList.csv", fstream::in);

					  char trash[500];
					  musiclist.getline(trash, 500);

					  for (it = mp3list.begin(); it != mp3list.end(); it++)
					  {
						  if (musiclist.peek() == ',')
						  {
							  musiclist.ignore();
						  }
						  else
						  {
							  musiclist.get(artist, 33, ',');
							  musiclist.ignore();
						  }
						  if (musiclist.peek() == ',')
						  {
							  musiclist.ignore();
						  }
						  else
						  {
							  musiclist.get(title, 33, ',');
							  musiclist.ignore();
						  }
						  musiclist.getline(trash, 500);
						  string tempart(artist);
						  (*it)->setArtist(checkInput(tempart));
						  string temptit(title);
						  (*it)->setTitle(checkInput(temptit));
						  if (((*it)->getArtist().empty() != true) && ((*it)->getTitle().empty() != true))
						  {
							  string temppathold = (*it)->getPathname();
							  string temppathnew = (*it)->getArtist() + " - " + (*it)->getTitle() + ".mp3";
							  const char *oldname = temppathold.c_str();
							  const char *newname = temppathnew.c_str();
							  rename(oldname, newname);
						  }
					  }
					  musiclist.close();
					  remove("MyMusicList.csv");
					  createMyMusicList(getMusicList(folderpathvs));
					  break;
			}
			}

		}
			system("pause");
			cout << endl;
	}
	return 0;
}

vector <MP3 *> getMusicList(CAtlString musicfolder)
{
	char tempconvert[501];
	WIN32_FIND_DATA mp3file;
	HANDLE filepos = FindFirstFile(_T("*.mp3"), &mp3file);


	wcstombs_s(NULL, tempconvert, 500, musicfolder, 500);
	string folderpath(tempconvert);
	vector <MP3*> mp3list;
	
	wcstombs_s(NULL, tempconvert, 500, mp3file.cFileName, 500);
	string filenamestring(tempconvert);
	string fullpath(folderpath + filenamestring);
	mp3list.push_back(new MP3(fullpath));

	while (FindNextFile(filepos, &mp3file) != 0)
	{
		wcstombs_s(NULL, tempconvert, 500, mp3file.cFileName, 500);
		string filenamestring(tempconvert);
		string fullpath(folderpath + filenamestring);
		mp3list.push_back(new MP3(fullpath));
	}

	FindClose(filepos);
	return mp3list;
}

string allowCommas(string input)
{
	input.insert(0, "\"");
	if (input.size() == 31)
	{
		input.erase(input.size()-1, 1);
	}
	input.insert(input.size(),"\"");
	return input;
	
}

string checkInput(string input)
{
	size_t found = input.find('"');
	while (found != string::npos)
	{
		input.erase(found,1);
		found = input.find('"');
	}
	while (input.length() > 30)
	{
		input.erase(input.length()-1, 1);
	}
	return input;
}

int DisplayContinueMessageBox()
{
	int msgboxID = MessageBox(
		NULL,
		L"Is the Music List up to date?\n Changes will be irreversable.\nClick Yes to Continue",
		L"Are you sure",
		MB_ICONEXCLAMATION | MB_YESNO
		);
	return msgboxID;
}

void createMyMusicList(vector <MP3 *> mp3list)
{
	vector <MP3*>::iterator it;
	ifstream mp3filestream;

	for (it = mp3list.begin(); it != mp3list.end(); it++)
	{
		mp3filestream.open((*it)->getPathname());
		mp3filestream.seekg(-128, ios_base::end);
		char tag[4];
		char extractionchar[31];
		mp3filestream.get(tag, 4);
		if (strcmp("TAG", tag) == 0)
		{
			mp3filestream.get(extractionchar, 31);
			string extracttitle(extractionchar);
			(*it)->setTitle(extracttitle);
			mp3filestream.get(extractionchar, 31);
			string extractartist(extractionchar);
			(*it)->setArtist(extractartist);
			mp3filestream.get(extractionchar, 31);
			string extractalbum(extractionchar);
			(*it)->setAlbum(extractalbum);
		}

		mp3filestream.close();
	}

	ofstream musiclist;
	musiclist.open("MyMusicList.csv", fstream::out);
	musiclist << "Artist,Title,Album,Filepath (Do Not Alter)" << endl;
	for (it = mp3list.begin(); it != mp3list.end(); it++)
	{
		musiclist << allowCommas((*it)->getArtist()) << "," << allowCommas((*it)->getTitle()) << "," << allowCommas((*it)->getAlbum()) << "," << allowCommas((*it)->getPathname()) << endl;
	}
	musiclist.close();
}