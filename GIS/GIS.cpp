#include "Interfaces.h"
#include "Graph.h"
#include "FibonacciHeap.h"
#include "Utility.h"

#include <boost\program_options.hpp>
#include <boost\filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

typedef po::options_description ProgramOptions;

string option_help = "help";
string option_graph_file = "gfile";
string option_coords_file = "cfile";

void testRootList(GraphPtr& graph)
{
	FibonacciHeap heap{};

	auto& vertices = graph->getVertices();

	double fake_weight = 256;
	for (auto& vert : vertices)
	{
		(fake_weight > 512) ? fake_weight -= 128 : fake_weight += 196;
		heap.insert(vert.second, (long)fake_weight);
		heap.printNodeList(std::cout, heap.peekMinElement());
		std::cout << "---------------------------------------------------" << endl;
	}

	std::cout << "-----------------END-------------------------------" << endl;
	//heap.printRootList(std::cout);
	//std::cout << "---------------------------------------------------" << endl;

	auto vert_ptr = heap.extractMin();
	heap.printNodeList(std::cout, heap.peekMinElement());
	vert_ptr = heap.extractMin();
	heap.printNodeList(std::cout, heap.peekMinElement());
	vert_ptr = heap.extractMin();
	heap.printNodeList(std::cout, heap.peekMinElement());
	vert_ptr = heap.extractMin();
	heap.printNodeList(std::cout, heap.peekMinElement());
	vert_ptr = heap.extractMin();
	std::cout << "-----------------EMPTY HEAP-------------------------------" << endl;
	heap.printNodeList(std::cout, heap.peekMinElement());
}

int main(int argc, char* argv[])
{
	ProgramOptions options("Program arguments");
	options.add_options()
		(option_help.c_str(), "Help message")
		(option_graph_file.c_str(), po::value<string>(), "File with graph")
		(option_coords_file.c_str(), po::value<string>(), "File with vertices coordinates");

	po::variables_map variables;
	po::store(po::parse_command_line(argc, argv, options), variables);
	po::notify(variables);

	fs::path graph_file;
	fs::path coords_file;
	
	if (variables.count(option_help.c_str()))
	{
		return 0;
	}

	if(variables.count(option_graph_file.c_str()))
	{
		graph_file = fs::path(variables[option_graph_file].as<string>());
	}
	else
	{
		return 1;
	}

	if (variables.count(option_coords_file.c_str()))
	{
		coords_file = fs::path(variables[option_coords_file].as<string>());
	}
	else
	{
		return 2;
	}

    return 0;
}



