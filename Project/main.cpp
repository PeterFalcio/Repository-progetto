#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

struct Fracture {
    int id;
    double x, y, z;
};

struct Trace {
    int id;
    int fractureId1, fractureId2;
    Fracture point1, point2;
};

std::vector<Trace> calculateTraces(std::vector<Fracture> fractures) {
    std::vector<Trace> traces;
    for (size_t i = 0; i < fractures.size(); ++i) {
        for (size_t j = i + 1; j < fractures.size(); ++j) {
            Trace trace;
            trace.id = traces.size() + 1;
            trace.fractureId1 = fractures[i].id;
            trace.fractureId2 = fractures[j].id;
            trace.point1 = fractures[i];
            trace.point2 = fractures[j];
            traces.push_back(trace);
        }
    }
    return traces;
}

void writeTracesToFile(const std::vector<Trace>& traces, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    outFile << "# Number of Traces\n";
    outFile << traces.size() << "\n";

    for (const auto& trace : traces) {
        outFile << "# TraceId; FractureId1; FractureId2; X1; Y1; Z1; X2; Y2; Z2\n";
        outFile << trace.id << "; " << trace.fractureId1 << "; " << trace.fractureId2 << "; "
                << trace.point1.x << "; " << trace.point1.y << "; " << trace.point1.z << "; "
                << trace.point2.x << "; " << trace.point2.y << "; " << trace.point2.z << "\n";
    }

    outFile.close();
}

int main() {
    std::vector<std::string> files = {"FR3_data", "FR10_data"};
    for (const auto& file : files) {
        std::ifstream inFile(file);
        if (!inFile.is_open()) {
            std::cerr << "Error: Unable to open file " << file << std::endl;
            continue;
        }

        std::vector<Fracture> fractures;
        Fracture fracture;
        while (inFile >> fracture.id >> fracture.x >> fracture.y >> fracture.z) {
            fractures.push_back(fracture);
        }

        std::vector<Trace> traces = calculateTraces(fractures);

        std::string outputFilename = file + "_traces.txt";
        writeTracesToFile(traces, outputFilename);

        inFile.close();
    }

    return 0;
}
