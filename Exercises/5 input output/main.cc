#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

int main (int argc, char** argv) {
    //Allows input such as main -n 1 -n 2 (so arguments) and echo "-n 1 -n 2 -n 3 -n 4 -n 5" > inputfile       ./main $(cat inputfile) > out.txt
	std::vector<double> numbers;
    std::string infile = "", outfile = "";
	for(int i=0;i<argc;++i){
		std::string arg=argv[i];
		if (arg=="-n" && i+1<argc)
			numbers.push_back(std::stod(argv[i+1]));
        if (arg == "--input" && i+1 < argc) infile = argv[i+1];
        if (arg == "--output" && i+1 < argc) outfile = argv[i+1];
	}

for(auto n: numbers)
std::cout << n <<" "<< std::sin(n) <<" "<< std::cos(n) <<"\n";

double x;
while( std::cin >> x ){
    std::cout << x <<" "<< std::sin(x) <<" "<< std::cos(x) << "\n";
    }
    
std::ifstream myinput(infile); //definerer myinput som in-file-stream objekt med infile som path
std::ofstream myoutput(outfile);
// Allows just giving numbers so echo 1 2 3 4 5 > input.txt       cat input.txt | ./main > out.txt, etc.    

double y;
if (myinput.is_open() && myoutput.is_open()){
    while (myinput >> y){
        myoutput << y << " " << std::sin(y) << " " << std::cos(y) << "\n";
    }
}
else if (!(infile == "") && !(outfile == "")) {
    std::cerr << "Error opening files: " << infile << outfile << "\n";
    return EXIT_FAILURE;
}

myinput.close();
myoutput.close();

exit(EXIT_SUCCESS);
}