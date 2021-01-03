# Run Profiles

## About Run Profiles

Run profiles allow us to change values used by the software without needing to rebuild the entire program. Software parameters are loaded at runtime during the initialization call. All software parameters are defined in the [software parameters header](../embedded/app/include/software_parameters.h) and are instantiated / loaded in the [load software parameters function](../embedded/app/src/load_software_parameters.c). 

## Profiles Formatting

Run profiles follow the following naming convention:

```
VARIABLE: <VALUE>
```

Please note that there is NOT a correlation between the variable name and value. The software assigns values read from the file to software variables under the assumption that the order in which software parameters appear and the header and the profile txt file match.

To add a software parameter, complete the following steps:

**1)** Add an extern declaration to the respective spot in the parameter header. @location annotation is recommended but not required. 

```
/**
 * Maximum duration for crawl state, FSM transitions to post-run if exceeded. Units us.
 *
 * @location states.c
 */
extern volatile int32_t MAXIMUM_CRAWL_TIME;
```

**2)** Add a pointer to the parameter to `PARAMETER_ARR`. The order of `PARAMETER_ARR` MUST match the order in which the parameters are declared & listed in the file.

**3)** Instantiate the parameter in the global namespace in `load_software_parameters()`. Instantiate it to a safe default value.

**4)** Add the parameter to your run profile using the format mentioned above. Remember - order matters.

## Validating Run Profiles

To assist in validating run profiles, a utility python tool has been created: [Validate Run Profile](../embedded/utils/validate_run_profile.py)

This script takes in an optional command line argument: A filepath to a parameter to verify. If none is provided, the default run profile (`run_profiles/active_run_profile.txt`) will be used.