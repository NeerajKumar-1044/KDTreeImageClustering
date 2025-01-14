#include <iostream>
#include <vector>
#include <string>
#include <dirent.h> 
#include <sys/stat.h>
#include <algorithm>
#include <fstream>
#include "json.hpp" // Include nlohmann JSON library

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
using json = nlohmann::json;

// Function to calculate the color histogram of an image
vector<double> calculateColorHistogram(const string &imagePath)
{
    int width, height, channels;
    unsigned char *img = stbi_load(imagePath.c_str(), &width, &height, &channels, 0);
    if (!img)
    {
        cerr << "Error loading image: " << stbi_failure_reason() << " for " << imagePath << endl;
        return vector<double>(3 * 256, 0); // Return zero histogram
    }

    int histogram[3][256] = {0};

    for (int i = 0; i < width * height; i++)
    {
        int r = img[i * channels + 0]; 
        int g = img[i * channels + 1];
        int b = img[i * channels + 2];

        histogram[0][r]++;
        histogram[1][g]++;
        histogram[2][b]++;
    }

    stbi_image_free(img);

    vector<double> normalizedHistogram(3 * 256);
    double totalPixels = width * height;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 256; i++)
        {
            normalizedHistogram[c * 256 + i] = histogram[c][i] / totalPixels;
        }
    }

    return normalizedHistogram;
}

bool hasValidImageExtension(const string &fileName)
{
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == string::npos)
    {
        return false;
    }

    string extension = fileName.substr(dotPos + 1);
    transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return (extension == "jpg" || extension == "png" || extension == "jpeg");
}

int main()
{
    const string directoryPath = "Images"; // Path to images directory
    const string outputFilePath = "output.json"; // Output JSON file

    DIR *dir;
    struct dirent *ent;

    json root; // Root JSON object to store histograms

    if ((dir = opendir(directoryPath.c_str())) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName = ent->d_name;
            if (fileName == "." || fileName == ".." || fileName[0] == '.')
                continue;

            if (hasValidImageExtension(fileName))
            {
                string imagePath = directoryPath + "/" + fileName;
                cout << "Processing image: " << imagePath << endl;

                vector<double> histogram = calculateColorHistogram(imagePath);

                if (!histogram.empty())
                {

                    root[fileName] = histogram; // Add histogram data as a single array under the image filename
                }
                else
                {
                    cerr << "No histogram data available for: " << imagePath << endl;
                }

                cout << "Finished processing image: " << imagePath << endl;
            }
            else
            {
                cout << "Skipping non-image file: " << fileName << endl;
            }
        }
        closedir(dir);
    }
    else
    {
        cerr << "Error opening directory: " << directoryPath << endl;
        return 1;
    }

    // Write histogram data to JSON file
    ofstream outputFile(outputFilePath);
    if (outputFile.is_open())
    {
        outputFile << root.dump(4); // Indent JSON output with 4 spaces for readability
        outputFile.close();
        cout << "Histogram data saved to: " << outputFilePath << endl;
    }
    else
    {
        cerr << "Error opening output file: " << outputFilePath << endl;
    }

    cout << "Processing complete!" << endl;
    return 0;
}
