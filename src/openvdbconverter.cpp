#include <openvdb/tools/LevelSetSphere.h> // replace with your own dependencies for generating the OpenVDB grid
#include "nanovdb/util/OpenToNanoVDB.h" // converter from OpenVDB to NanoVDB (includes NanoVDB.h and GridManager.h)
#include "nanovdb/util/IO.h"
// Convert an openvdb level set sphere into a nanovdb, use accessors to print out multiple values from both
// grids and save the NanoVDB grid to file.
// Note, main depends on BOTH OpenVDB and NanoVDB.
int main(int argc, char* argv[]) {
    try {
        openvdb::initialize();
        // Create a VDB file object.
        openvdb::io::File file(argv[1]);
        // Open the file.  This reads the file header, but not any grids.
        file.open();
        // Loop over all grids in the file and retrieve a shared pointer
        // to the one named "LevelSetSphere".  (This can also be done
        // more simply by calling file.readGrid("LevelSetSphere").)
        openvdb::GridBase::Ptr baseGrid;
        for (openvdb::io::File::NameIterator nameIter = file.beginName();
            nameIter != file.endName(); ++nameIter)
        {
            // Read in only the grid we are interested in.
            if (nameIter.gridName() == "density") {
                baseGrid = file.readGrid(nameIter.gridName());
            } else {
                std::cout << "skipping grid " << nameIter.gridName() << std::endl;
            }
        }
        file.close();
        
        openvdb::FloatGrid::Ptr srcGrid = openvdb::gridPtrCast<openvdb::FloatGrid>(baseGrid);
        // Create an OpenVDB grid (here a level set surface but replace this with your own code)
        // auto srcGrid = openvdb::tools::createLevelSetSphere<openvdb::FloatGrid>(100.0f, openvdb::Vec3f(0.0f), 1.0f);
        // Convert the OpenVDB grid, srcGrid, into a NanoVDB grid handle.
        auto handle = nanovdb::openToNanoVDB(*srcGrid);
        // Define a (raw) pointer to the NanoVDB grid on the host. Note we match the value type of the srcGrid!
        auto* dstGrid = handle.grid<float>();
        if (!dstGrid)
            throw std::runtime_error("GridHandle does not contain a grid with value type float");
        // Get accessors for the two grids. Note that accessors only accelerate repeated access!
        // auto dstAcc = dstGrid->getAccessor();
        // auto srcAcc = srcGrid->getAccessor();
        // // Access and print out a cross-section of the narrow-band level set from the two grids
        // for (int i = 97; i < 104; ++i) {
        //     printf("(%3i,0,0) OpenVDB cpu: % -4.2f, NanoVDB cpu: % -4.2f\n", i, srcAcc.getValue(openvdb::Coord(i, 0, 0)), dstAcc.getValue(nanovdb::Coord(i, 0, 0)));
        // }
        nanovdb::io::writeGrid("sphere.nvdb", handle); // Write the NanoVDB grid to file and throw if writing fails
    }
    catch (const std::exception& e) {
        std::cerr << "An exception occurred: \"" << e.what() << "\"" << std::endl;
    }
    return 0;
}