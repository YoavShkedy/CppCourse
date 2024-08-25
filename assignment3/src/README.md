To build the whole project:
1. Navigate to the src/ directory
2. Run main_build_script.sh  

To build Algorithm_206448649_314939398_A:
1. Navigate to the src/Algorithm_206448649_314939398_A/ directory
2. Run: algo_a_build_script.sh

To build Algorithm_206448649_314939398_B:
1. Navigate to the src/Algorithm_206448649_314939398_B/ directory
2. Run: algo_b_build_script.sh  

The delete_and_rebuild.sh script will:
- Delete every '.csv' file in the working directory
- Delete every '.error' file in the working directory
- Delete the 'build' directory
- Build the whole project

Executable Running example:  
-house_path=<path_to_house_files> -algo_path=house_path=<path_to_algorithm_files> -summary_only -num_threads=7