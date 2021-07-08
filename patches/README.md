# Patches Folder

This folder contains patch files that should be applied in order to enable an specific feature.

## Patches List and Instructions

Each patch file is specfic to a folder and the instructions regarding then are shown below.

* `mocked_uart_apply_over_idf_path.patch`

    * This patch is related to the mocking of the uart's driver in order to perform unit tests. In order to mock the uart's driver component (CMock style), it is necessary to add the `mock` folder with its `mock_config.yaml`. Also, it is necessary to change the `CMakeLists.txt` to build the mock instead actual code.
    * This patch should be applied over the IDF path (which should be previously checked out to `v4.3`)
