#include "lastline.h"

void getlastlineFast(const string& filename, string& lastline)
{
	std::ifstream fs;
	fs.open(filename.c_str(), std::fstream::in);
	if (fs.is_open())
	{
		//Got to the last character before EOF
		fs.seekg(-1, std::ios_base::end);
		if (fs.peek() == '\n')
		{
			//Start searching for \n occurrences
			fs.seekg(-1, std::ios_base::cur);
			int i = fs.tellg();
			for (i; i > 0; i--)
			{
				if (fs.peek() == '\n')
				{
					//Found
					fs.get();
					break;
				}
				//Move one character back
				fs.seekg(i, std::ios_base::beg);
			}
		}
		getline(fs, lastline);
	}
}