#include <liblas/cstdint.hpp>
#include <liblas/detail/utility.hpp>

#include <liblas/laswriter.hpp>
#include <liblas/laspoint.hpp>
#include <liblas/lascolor.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/lasheader.hpp>
#include <liblas/lasindex.hpp>

#ifdef HAVE_SPATIALINDEX
#include <spatialindex/SpatialIndex.h>

#endif
#include <iostream>
#include <fstream>
#include <vector>

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

std::istream* OpenInput(std::string filename) 
{
    std::ios::openmode const mode = std::ios::in | std::ios::binary;
    std::istream* istrm;
    if (compare_no_case(filename.c_str(),"STDIN",5) == 0)
    {
        istrm = &std::cin;
    }
    else 
    {
        istrm = new std::ifstream(filename.c_str(), mode);
    }
    
    if (!istrm->good())
    {
        delete istrm;
        throw std::runtime_error("Reading stream was not able to be created");
        exit(1);
    }
    return istrm;
}

void usage() {}

using namespace liblas;

int main(int argc, char* argv[])
{
    int rc = 0;

    std::string input;
    std::string output;
    
    for (int i = 1; i < argc; i++)
    {
        if (    strcmp(argv[i],"-h") == 0 ||
                strcmp(argv[i],"--help") == 0
            )
        {
            usage();
            exit(0);
        }
        else if (   strcmp(argv[i],"--input") == 0  ||
                    strcmp(argv[i],"-input") == 0   ||
                    strcmp(argv[i],"-i") == 0       ||
                    strcmp(argv[i],"-in") == 0
                )
        {
            i++;
            input = std::string(argv[i]);
        }
        else if (   strcmp(argv[i],"--output") == 0  ||
                    strcmp(argv[i],"--out") == 0     ||
                    strcmp(argv[i],"-out") == 0     ||
                    strcmp(argv[i],"-o") == 0       
                )
        {
            i++;
            output = std::string(argv[i]);
        }
        else if (i == argc - 2 && output.empty() && input.empty())
        {
            input = std::string(argv[i]);
        }
        else if (i == argc - 1 && output.empty() && input.empty())
        {
            input = std::string(argv[i]);
        }
        else if (i == argc - 1 && output.empty() && input.empty())
        {
            output = std::string(argv[i]);
        }
        else 
        {
            usage();
            exit(1);
        }
    }
    
    if (input.empty()) {
        usage();
        exit(-1);
    }
    std::cout << "input: " << input<<  " output: " <<output<<std::endl;
    // 
    std::istream* istrm = OpenInput(input);
    LASReader* reader = new LASReader(*istrm);
    
    
    LASDataStream* idxstrm = new LASDataStream(reader);

    LASIndex* index = new LASIndex(*idxstrm, input);
    
    delete idxstrm;
    delete index;
    delete reader;
    
    LASIndex* idx = new LASIndex(input);
    
    std::vector<liblas::uint32_t>* ids = 0;

    try{
//        ids = idx->intersects(289815.12,4320979.06, 289818.01,4320982.59,46.83,170.65);

// _zoom
//        ids = idx->intersects(630355.0,4834609.0,630395.0,4834641.0,0.0,200.0);

// _clip
// 630346.830000,4834500.000000,55.260000
        ids = idx->intersects(630262.300000,4834500.000000,630346.830000,4834500.000000,50.900000,55.260000);
//       ids = idx->intersects(630297.0,4834497.0,630302.0,4834501.0,0.0,200.0);

    } catch (Tools::Exception& e) {
        std::string s = e.what();
        std::cout << "error querying index value" << s <<std::endl; exit(1);
    }
        
    
    
    std::cout << "Vec length" << ids->size() << std::endl;  
    delete idx;
  
    // LASPoint* p
    // liblas::uint32_t num_points = reader->GetHeader().GetPointRecordsCount();
    // for (int i =0; i < num_points ; i++) {
    //     
    //     bool bInserted = index->insert(*p, i);
    // }

    // reader->Index(input);
    // 
    // LASHeader header = reader->GetHeader();
    // std::cout << "number of points: " << header.GetPointRecordsCount() << std::endl;
    // 
    // for (int i=0; i< header.GetPointRecordsCount(); i++) {
    //     
    //     try{
    //         bool read = reader->ReadPointAt(i);
    //         LASPoint p = reader->GetPoint();
    //     } catch (Tools::IllegalStateException& e) {
    //         std::string s = e.what();
    //         std::cout << "error creating index" << s <<std::endl; exit(1);
    //     }        
    //     
    //     std::cout.precision(2);
    //     std::cout.setf(std::ios_base::fixed);
    //     // std::cout << "x: " << p.GetX() << " y: " << p.GetY() << std::endl;
    // }
    // 
    // LASIndex* idx = reader->GetIndex();
    // 
    // std::vector<liblas::uint32_t>* ids = 0;
    // try{
    //     ids = idx->intersects(289815.12,4320979.06, 289818.01,4320982.59,46.83,170.65);
    // } catch (Tools::IllegalArgumentException& e) {
    //     std::string s = e.what();
    //     std::cout << "error querying index value" << s <<std::endl; exit(1);
    // }
    //     
    // 
    // 
    // std::cout << "Vec length" << ids->size() << std::endl;
    // delete reader;
    // delete istrm;
}