#!/usr/bin/python

import sys
import os
import re

# Get path of python script 
dir_path = os.path.dirname(os.path.realpath(__file__))

# Locations for both software_paramaters header & run_profile to test
software_parameters_h_location = dir_path + "/../app/include/software_parameters.h"
run_profile_location = dir_path + "/../../run_profiles/active_run_profile.txt"

# Verify 0 or 1 passed-in argument 
# Arg 0 is always the executable location
if len(sys.argv) > 2:
    print("Invalid number of arguments. 0 or 1 are accepted")

# If 1 argument is passed, it's a path to a different run_profile
if len(sys.argv) == 2:
    # Set the run_profile location to the path input, based off the directory the command was run in
    run_profile_location = os.getcwd() + "/" + sys.argv[1]

# Parameter list - Build list in-order
parameter_list = []

# Open header file
parameter_header = open(software_parameters_h_location,'r')

# Iterate through file line-by-line
for line in parameter_header.readlines():
    # Check if line starts with "extern volatile"
    # A little janky, but it works for now
    if line.startswith("extern volatile"):
        # Grab last word of the line, minus the ;. That's the parameter name
        # Append it to the list
        variable_name = line.split()[-1][:-1]
        parameter_list.append(variable_name)

# Close file
parameter_header.close()

# Open run profile
run_profile = open(run_profile_location, 'r')

# Set up regex pattern for expected format
# Should be capable of accepting int/string types
pattern = re.compile("^(?:\w)+(?: |\t)*:(?: |\t)*(?:\")?(?:\w)+(?:\")?")

# Counter to hold array position
position = 0

# Counter to hold number of parameters in the run_profile file
file_counter = 0

# Success status
success_status = True

# Iterate through file line-by-line
for line in run_profile.readlines():
    # If line is valid by matching regex
    if pattern.match(line):
        variable_name = line.split()[0]

        # If counter is higher than length of parameter list, set success status to false
        if position >= len(parameter_list):
            success_status = False

        # If there's a mismatch, print for user
        if success_status is True and variable_name != parameter_list[position]:
            print("Run profile mismatch!")
            print("Expected: " + parameter_list[position])
            print("Found: " + variable_name)

            # Set success status to false
            success_status = False

        # Increment counter
        position = position + 1

if success_status is True:
    print("Valid run profile found!")
elif position != len(parameter_list):
    print("Number of elements mismatch!")
    print("Number of elements in header file: " + str(len(parameter_list)))
    print("Number of elements in run profile file: " + str(position))