#include <iostream>
#include <vector>
#include <string>
#include <dirent.h> 
#include <sys/stat.h>
#include <algorithm>
#include <cstdio> 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

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

    // Initialize histogram for R, G, B channels
    int histogram[3][256] = {0};

    // Calculate the histogram
    for (int i = 0; i < width * height; i++)
    {
        int r = img[i * channels + 0]; 
        int g = img[i * channels + 1];
        int b = img[i * channels + 2];

        histogram[0][r]++;
        histogram[1][g]++;
        histogram[2][b]++;
    }

    // Free the image memory
    stbi_image_free(img);

    // Normalize the histogram
    vector<double> normalizedHistogram(3 * 256);
    double totalPixels = width * height;

    for (int c = 0; c < 3; c++)
    {
        for (int i = 0; i < 256; i++)
        {
            normalizedHistogram[c * 256 + i] = histogram[c][i] / totalPixels; // Normalization
        }
    }

    return normalizedHistogram;
}

void appendHistogramToFile(const vector<double> &histogram, const string &outputPath, const string &imageName)
{
    FILE *file = fopen(outputPath.c_str(), "a");
    if (!file)
    {
        cerr << "Error opening file for appending: " << outputPath << endl;
        return;
    }

    fprintf(file, "Histogram for %s:\n", imageName.c_str());

    // Write histogram values for each channel
    for (int c = 0; c < 3; c++)
    {
        fprintf(file, "Channel %d:\n", c); // Add channel label
        for (int i = 0; i < 256; i++)
        {
            fprintf(file, "%f", histogram[c * 256 + i]); // Write histogram value
            if (i < 255)
                fprintf(file, " ");
        }
        fprintf(file, "\n");
    }

    fprintf(file, "\n"); 
    fclose(file);    
    cout << "Appended histogram data to: " << outputPath << endl;
}

bool hasValidImageExtension(const string &fileName)
{
    size_t dotPos = fileName.find_last_of('.');
    if (dotPos == string::npos)
    {
        return false;
    }

    string extension = fileName.substr(dotPos + 1);
    // Convert extension to lowercase for comparison
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    return (extension == "jpg" || extension == "png" || extension == "jpeg");
}

int main()
{
    // write the directory path that contain images
    const string directoryPath = "C:/Users/mohim/OneDrive/Pictures/Saved Pictures/TestImages";

    const string histogramOutputPath = directoryPath + "/histograms.txt"; // File to save histogram vectors

    cout << "Opening directory: " << directoryPath << endl;

    DIR *dir;
    struct dirent *ent;

    // Attempt to open the directory
    if ((dir = opendir(directoryPath.c_str())) != NULL)
    {
        // Loop through all entries in the directory
        while ((ent = readdir(dir)) != NULL)
        {
            string fileName = ent->d_name;
            cout << "Found file: " << fileName << endl; // Log each file found

            // Skip hidden files and directories
            if (fileName == "." || fileName == ".." || fileName[0] == '.')
            {
                cout << "Skipping hidden or system file: " << fileName << endl;
                continue;
            }

            // Check for valid image files
            if (hasValidImageExtension(fileName))
            {
                string imagePath = directoryPath + "/" + fileName;
                cout << "Processing image: " << imagePath << endl;

                vector<double> histogram = calculateColorHistogram(imagePath); // Calculate the histogram

                // Check if histogram is valid
                if (!histogram.empty())
                {
                    // Append the histogram to the specified file at the end
                    appendHistogramToFile(histogram, histogramOutputPath, fileName);
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

    cout << "Processing complete!" << endl;
    return 0;
}