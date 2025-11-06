#include "bml-math.h"
#include "file-tools.h"
#include <string>
#include <sstream>


bool testArange(std::string directoryOfTruth)
{
    for (size_t i = 0; i < 5; i++)
    {
        // Read coefficients
        std::stringstream coeffFile;
        coeffFile << directoryOfTruth << "/bml-arange-truth-coeffs-" << i;
        std::vector<double> coeffs = BML::FileTools::readVecFromCsv(coeffFile);

        // Read python array
        std::stringstream truthFile;
        truthFile << directoryOfTruth << "/bml-arange-truth-arr-" << i;
        std::vector<double> pyArr = BML::FileTools::readVecFromCsv(truthFile);

        // Build C++ array
        std::vector<double> cppArr = BML::Math::arange(coeffs[0], coeffs[1], coeffs[2]);

        // Check for accuracy
        if (cppArr.size() != pyArr.size())
            return false;

        auto cppIter = cppArr.begin();
        for (auto pyIter = pyArr.begin(); pyIter != pyArr.end(); pyIter++, cppIter++)
        {
            // Get difference in values
            double diff = np.abs(*pyIter - *cppIter);
            if (diff >= 0.0001)
                return false;
        }
    }

    return true;
}