# filters.tpu

![](doc/img/flightline111-StdZ-large.png)

A [PDAL](https://pdal.io/index.html) filter to generate per-point total propagated uncertainty (TPU) for airborne laser scanning (ALS) point clouds when supplied:

1. An ALS point cloud of a single flightline
2. A trajectory for the flightline.
3. A JSON file containing sensor measurement standard deviations (uncertainties).

The generated TPU consists of 3x3 covariance matrix elements added as extra dimensions to each point: x, y, and z variances (diagonal elements) and xy, xz, and yz covariances (symmetric off-diagonal elements).

The filter is only applicable to data generated by ALS systems with oscillating (sawtooth ground pattern) or rotating (parallel line ground pattern) mirrors.


## Generic Pipeline Example

```json
[
    {
        "type": "readers.las",
        "filename": "my-flightline.laz",
        "tag": "cloud"
    },
    {
        "type": "readers.sbet",
        "filename": "my-flightline-trajectory.sbet",
        "tag": "trajectory"
    },
    {
        "type": "filters.als_tpu",
        "measurement_stdev": "my-sensor-uncertainties.json",
        "inputs": [
            "cloud",
            "trajectory"
        ],
        "extended_output": "true"
    },
    {
        "type": "writers.las",
        "minor_version": 4,
        "extra_dims": "all",
        "filename": "my-flightline-tpu.laz"
    }
]
```


## Generic JSON Measurement Uncertainty Example

Angular values must be given in degrees, linear units in meters, and laser beam divergence must use the $1/e^2$ definition and be given in milliradians. The filter only uses the `name` and `value` keys in the `uncertainties` array; the remaining keys are an optional means to document the provenance of the uncertainty values.

```json
{
    "system": "Make and model of my ALS laser scanner and inertial navigation system",
    "uncertainties": [
        {
            "name": "std_lidar_range",
            "source": "My laser scanner datasheet",
            "value": 0.008
        },
        {
            "name": "std_scan_angle",
            "source": "my laser scanner datasheet",
            "value": 0.001
        },
        {
            "name": "std_sensor_xy",
            "source": "my inertial navigation system datasheet",
            "value": 0.01
        },
        {
            "name": "std_sensor_z",
            "source": "my inertial navigation system datasheet",
            "value": 0.02
        },
        {
            "name": "std_sensor_rollpitch",
            "source": "my inertial navigation system datasheet",
            "value": 0.005
        },
        {
            "name": "std_sensor_yaw",
            "source": "my inertial navigation system datasheet",
            "value": 0.007
        },
        {
            "name": "std_bore_rollpitch",
            "source": "my survey metadata, rule of thumb, or custom source",
            "value": 0.001
        },
        {
            "name": "std_bore_yaw",
            "source": "my survey metadata, rule of thumb, or custom source",
            "value": 0.004
        },
        {
            "name": "std_lever_xyz",
            "source": "my survey metadata, rule of thumb, or custom source",
            "value": 0.02
        },
        {
            "name": "beam_divergence",
            "source": "my laser scanner datasheet",
            "value": 0.49
        }
    ]
}
```


## Options

* **include_inc_angle:** Boolean flag to include the influence of the laser beam to ground incidence angle in the TPU. Requires normal vectors on each point (see [`filters.normal`](https://pdal.io/stages/filters.normal.html?highlight=filters%20normal)). [Default=true]

* **max_inc_angle::** Maximum allowable incidence angle specified in degrees and less than 90 degrees. Incidence angles approaching 90 degrees will produce very large TPU values. [Default=85.0]

* **no_data_value:** Value to assign to TPU dimensions when trajectory information is not available. [Default=-1.0]]
        
* **extended_output:** Option to export inverted range and scan angle measurements, interpolated trajectory position and attitude values, and coordinate standard deviations (just the square root of the x, y, and z variances; a convenience). [Default=false]


## Installation

Only tested on Linux via Windows WSL2 with a Conda environment.

### Dependencies

1. PDAL
    * Install into your Conda environment: `conda install -c conda-forge pdal`
    * Tell CMake to look inside the Conda environment for the PDAL package: `conda env config vars set CMAKE_MODULE_PATH=${CONDA_PREFIX}`
2. nlohmann's JSON library
    * Install into your Conda environment: `conda install -c conda-forge nlohmann_json`

### Build and Install:

Clone this repository, navigate to the main directory, and:
```
mkdir build && cd build
cmake -D CMAKE_INSTALL_PREFIX=${CONDA_PREFIX} ..
cmake ..
make
sudo make install
```

Check that the plugin is installed:
```bash
$ pdal --drivers | grep als_tpu
filters.als_tpu              Per-point airborne lidar Total Propagated Uncertainty (TPU) via a generic sensor model
```


## Additional Detail and Example Workflows

The general approach of the algorithm is detailed [here](doc/details.md). A detailed white paper with the exact math and assumptions used in the algorithm is forthcoming.

A complex example that starts with imperfect tiled LAS point cloud data from a triple channel ALS and works through the process of generating a clean set of LAZ tiles cropped to an area of interest and containing per-point TPU can be found [here](doc/example/example.md). A simpler example should be created as a first step for new users.
