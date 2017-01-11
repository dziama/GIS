#include "Interfaces.h"
#include "Graph.h"
#include "FibonacciHeap.h"
#include "Utility.h"
#include "HeapGui.h"
#include "SFML.h"

#include <boost\program_options.hpp>
#include <boost\filesystem.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

typedef po::options_description ProgramOptions;

//argumenty wywolania
string option_help = "help";
string option_graph_file = "gfile";
string option_coords_file = "cfile";

int main(int argc, char* argv[])
{
	try
	{
		//Obsluga argumentow wywolania
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

		if (variables.count(option_graph_file.c_str()))
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

		//Wczytanie czcionki
		string font_path = "E:/FUTRFW.TTF";
		Font font;
		bool result = font.loadFromFile(font_path);

		//Odczyt maciezy wag grafu
		ifstream graph_ifstream;

		graph_ifstream.open(graph_file.c_str());

		Matrix weight_matrix{};
		Graph graph{};
		FibonacciHeap heap{};

		if (graph_ifstream.is_open())
		{
			readMatrix(graph_ifstream, weight_matrix);

			vector<VertexId> vertex_ids;
			vector<EdgeId> edge_ids;

			for (unsigned i = 0; i < weight_matrix.size(); ++i)
			{
				vertex_ids.push_back(graph.addVertex());
			}

			for (unsigned i = 0; i < weight_matrix.size(); ++i)
			{
				for (unsigned j = 0; j < weight_matrix.size(); ++j)
				{
					if (weight_matrix[i][j] == 0L)
					{
						continue;
					}
					else if (i == j)
					{
						continue;
					}
					else
					{
						EdgeId id = graph.addEdge(vertex_ids[i], vertex_ids[j], weight_matrix[i][j]);
						if (id != -1)
						{
							edge_ids.push_back(id);
						}
					}
				}
			}

			//Inicjalizacja i uruchomienie GUI
			HeapGui gui{ heap, graph, font };
			gui.showGui();
		}
	}
	catch (exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}
	return 0;
}

