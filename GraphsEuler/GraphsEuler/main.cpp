#include "GraphRep.h"
#include "GraphsGen.hpp"

int main()
{
	std::ofstream eulFile, hamilFile;
	eulFile.open("wyniki/hamil05densall.txt");
	hamilFile.open("wyniki/hamilton3DensResult.txt");

	eulFile << "Vertices\tTime\n";
	hamilFile << "Vertices\tTime\n";

	//if (eulFile.is_open() && hamilFile.is_open());
	//{
		for (int i = 5; i <= 100; i += 1)
		{
			GraphGenerator eulerG(i, 0.5);
			std::string filename = "genfile.txt";
			eulerG.generateAMCoherentEulerGraph(filename);
			AdjacencyMatrix adj(filename);
			AdjacencyList adjlist(adj);
			auto start = std::chrono::steady_clock::now();
			std::cout << adjlist.findHamiltonianCycles() << std::endl;
			auto end = std::chrono::steady_clock::now();
			eulFile << i << "\t" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
		}
	//}
	eulFile.close();
	hamilFile.close();

	return 0;
}

