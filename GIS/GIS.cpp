#include "Interfaces.h"
#include "Graph.h"

#include <fstream>
#include <sstream>
#include <boost\program_options.hpp>
#include <boost\filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using std::stringstream;
using std::ifstream;
using std::getline;

typedef po::options_description ProgramOptions;
typedef vector<vector<EdgeWeight>> Matrix;

string option_help = "help";
string option_graph_file = "gfile";
string option_coords_file = "cfile";

void readMatrix(ifstream& file, Matrix& mat)
{
	string line;
	stringstream sstream;
	EdgeWeight weight;

	int row = 0;
	while (getline(file, line))
	{
		sstream << line;
		mat.push_back(vector<EdgeWeight>());
		weight = 0L;
		do
		{
			sstream >> weight;
			mat[row].push_back(weight);
		} while (sstream.eof() == false);
		++row;
		sstream = stringstream{};
	}

	for (auto& row : mat)
	{
		if (row.size() != mat.size())
		{
			break;
			//throw exe, matrix size incorrect
		}
	}
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

	ifstream graph_ifstream;
	ifstream coords_ifstream;

	graph_ifstream.open(graph_file.c_str());
	coords_ifstream.open(coords_file.c_str());

	Matrix weight_matrix{};
	Matrix coords_matrix{};

	if (graph_ifstream.is_open() && coords_ifstream.is_open())
	{
		readMatrix(graph_ifstream, weight_matrix);
		readMatrix(coords_ifstream, coords_matrix);

		GraphPtr main_graph{new Graph()};
		vector<VertexId> vertex_ids;
		vector<EdgeId> edge_ids;

		for (int i = 0; i < weight_matrix.size(); ++i)
		{
			vertex_ids.push_back(main_graph->addVertex());
		}

		for (int i = 0; i < weight_matrix.size(); ++i)
		{
			for (int j = 0; j < weight_matrix.size(); ++j)
			{
				main_graph->addEdge(vertex_ids[i], vertex_ids[j], weight_matrix[i][j]);
			}
		}
	}
	else
	{
		return 3;
	}

    return 0;
}


