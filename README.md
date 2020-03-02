# README #

### AFINES: Active Filament Network Simulation ###

##### as detailed in : #####
#### A versatile framework for simulating the dynamics mechanical structure of cytoskeletal networks ####

### Authors / Contributors ###

* Simon Freedman (University of Chicago)
* Shiladitya Banerjee (University College London)
* Glen Hocky (University of Chicago)
* Aaron Dinner (University of Chicago)

#### created at the University of Chicago ####

Edited by Daniel Seara (Yale), 02/06/2018

### Compiling AFINES on Grace cluster at Yale ###
* First ensure you have an account on Grace and have an SSH key loaded. Follow the instructions here if you have not done that yet, follow the instructions [here](https://research.computing.yale.edu/support/hpc/getting-started)

* ssh into grace using your netid
    ```
        $ ssh NETID@grace.hpc.yale.edu
    ```


* This system requires gcc+11 and Boost which you can load on Grace via the commands
    ```
        $ module load Boost/1.61.0-foss-2016b
    ```

* In order to point the compiler to the Boost library, set the following variable
    ```
        $ export BOOST_ROOT=/gpfs/loomis/apps/avx/software/Boost/1.61.0-foss-2016b/lib
    ```

* Download the simulation from github
    ```
        $ git clone https://github.com/dsseara/afines.git
    ```

* Change directories to the afines folder and make a new directory named `bin`
    ```
        $ cd afines
        $ mkdir bin
    ```

* Edit makefile using your favorite command line based editor (vim, emacs, etc) to add the following to line 20 (or whatever line starts with `INC`)
    ```
        $ -I /gpfs/loomis/apps/avx/software/Boost/1.61.0-foss-2016b/include
    ```

* If you don't already have an executable, run the command (flags in `[...]` are optional):
    ```
        $ make [clean] [tar] network
    ```
    * [clean] will delete the old executable
    * [tar] will generate the file tars/amxbd.tar.gz
    * IF this doesn't work, then there's probably a dependency or linker issue. Try each of the following solutions in the order prescribed, and attempt to compile in between.
        1. Make sure you have Boost installed
        2. Find the folder with the "*boost*.dylib" or "*boost*.a" folders; when I installed Boost using MacPorts, it was `/opt/local/lib/` . Run the command:

                `export BOOST_ROOT=<my boost folder>`

              With the Macports installation, `<my boost folder>=/opt/local/lib`.

        3. Within `BOOST_ROOT`, identify if the library folders have a suffix, such as "`-mt`" or "`-d`"
        (e.g., the program-options library on my Mac is named "`libboost_program_options-mt.dylib`", instead of "`libbbost_program_options.dylib`"). If so, run the command:

                `export BOOST_SUFFIX=<my boost suffix>`

             With the Macports installation, `<my boost suffix>=-`

        4. Find the folder with the boost/*.h files; with MacPorts installation, it was `/opt/local/include/`. Add `-I <myincludefolder>` to the line that begins `INC :=` in the makefile.

* You should now have an executable file called bin/afines. NOTE: you only need to recreate this file if you edit the source
  code.

### Running a simulation ###
*NOTE: When you sign onto Grace, you are on a "login node". DO NOT RUN SIMULATIONS HERE. Request a compute node. [See instructions here](https://research.computing.yale.edu/support/hpc/user-guide/slurm)*


Create an output directory for your simulation. e.g. on Grace you could use:
    ```
        $ mkdir ~/project/YYMMDD/SIMNAME
    ```
Replacing `YYMMDD` with today's date, and `SIMNAME` is whatever you want to call your simulation. *Note that `~/project` is actually a softlink to another location. Use the command `ls -alh ~` to see what the real path is. On my account it is `/project/fas/murrell/dss86`*.

There are two ways to run simulations, either directly from the command line, or by using a configuration file.

### Command line example###

For example, to run a 500 second of simulation of 10 rigid actin filaments, an active motor density of 0.5 and a crosslinker density of 0.05 you would enter the command:
    ```
        $ cd ~/afines/
        $ ./bin/afines --tf 500 --npolymer 10 --a_motor_density 0.5 --p_motor_density 0.05 --dir /project/fas/murrell/dss86/YYMMDD/SIMNAME
    ```
*Note that you have to be in the `afines` folder for `./bin/afines` to work. If you are not in that folder, specify the full path to the `bin/afines` executable instead.*

### Configuration file example###
Below is an example of a configuration file named `example.cfg`.
To run a simulation using this configuration, enter the command
    ```
        $ cd ~/afines/
        $ ./bin/afines -c /path/to/example.cfg
    ```
I recommend keeping `example.cfg` in whatever output directory the data will be stored in.

#### example.cfg ####
```
npolymer=500
nmonomer=11
dt=0.001
nframes=2000
tfinal=100
actin_length=0.5
a_motor_density=0.05
link_intersect_flag=true
actin_pos_str=0,0,0:1,2,3.141
dir=/project/fas/murrell/dss86/YYMMDD/SIMNAME
```

### Simulation Outputs ###

* Once your simulation has completed, the following files will have been generated in `~/project/YYMMDD/SIMNAME`:
 * txt_stack/actins.txt //the trajectories of every actin bead
 * txt_stack/links.txt //the trajectories of every link
 * txt_stack/amotors.txt //the trajectories of all active motors (e.g., myosin) at every time step
 * txt_stack/pmotors.txt //the trajectories of all passive motors (e.g., crosslinkers) at every time step
 * data/filament_e.txt //filament energetics
 * data/pe.txt //potential energy of all particles in simulation
 * data/config_full.cfg //full configuration used in simulation

All files are tab delimited

* txt_stack/actins.txt has the format

    * x y r idx
        * (x, y)  = bead position,
        * r  = bead radius
        * idx = index of filament that the bead is on

* txt_stack/links.txt has the format
     * x y dx dy idx
         * (x, y) = end position of link closer to the barbed end of filament
         * (x + dx, y + dy) = end position of link farther from barbed end
         * idx = index of filament the link is on

* txt_stack/amotors.txt and txt_stack/pmotors.txt have the format
    * x, y, dx, dy, fidx0, fidx1, lidx0, lidx1, s_stayUnbound, s_bind, s_unbind, s_stayBound, s_total
        * (x, y) = position of head 0 of motor
        * (x + dx, y + dy) = position of head 1 of motor
        * fidx0 = index of filament that head 0 is attached to (-1 if not attached)
        * fidx1 = index of filament that head 1 is attached to (-1 if not attached)
        * lidx0 = index of link that head 0 is attached to (-1 if fidx0 = -1)
        * lidx1 = index of link that head 1 is attached to (-1 if fidx1 = -1)
        * s_stayUnbound = irreversibility for motors that stay unbound
        * s_bind = irreversibility for steps that bind
        * s_unbind = irreversibility for steps that unbind
        * s_stayBound = irreversibility for steps that stay bound
        * s_total = total irreversibility for motor

* data/filament_e.txt is the energetics of each filament, and has the format
    * KE PE TE idx
        * KE = total v^2 of filament
        * PE = total potential energy of filament
        * TE = KE + PE
        * idx = filament index
The time associated with the positions/energies is on it's own line before
each list of positions within the file. Thus the structure of actins.txt is:
```
    t = t1
    x1, y1, r1, idx1
    .
    .
    .
    xn, yn, rn, idxn
    t=t2
    x1, y1, r1, idx1,
    .
    .
    .
    t=tn
    .
    .
    .
    xn, yn, rn, idxn
```

* data/pe.txt is the total potential energy of all particles at a given time step and has the format
    * U(filament_stretch)  U(filament_bend) U(xlink_stretch) U(motor_stretch) where each U is total at that timestep
    * time isn't delineated in these files; rather line 1 is t=t1, line 2, is t=t2, etc.

* data/config_full.cfg is the full set of configuration options used for the simulation. Thus if a simulation did not
  complete, you can restart with
```
./bin/afines -c data/config_full.cfg --restart true
```

## Simulation Parameters ##

|variable name              |type   |default value  |units  |description                        |
|-------------              |:----: |:-------------:|:-----:|-----------------------------------|
|**ENVIRONMENT**            |||||
|xrange                     |double |10             |um     |size of cell in horizontal direction|
|yrange                     |double |10             |um     |size of cell in vertical direction  |
|grid_factor                |double |2              |um^(-1)|number of grid boxes per micron     |
|dt                         |double |0.0001         |s      |length of individual timestep       |
|tinit                      |double |0              |s      |time that recording of simulation starts|
|tfinal                     |double |0.01           |s      |length of simulation|
|nframes                    |int    |1000           |0      |number of frames of actin/link/motor positions printed to file (equally distant in time)|
|nmsgs                      |int    |10000          |0      |number of timesteps between printing simulation progress to stdout|
|viscosity                  |double |0.001          |mg/um*s|Dynamic viscosity|
|temperature                |double |0.004          |pN*um  |Temp in energy units |
|bnd_cnd                    |string |"PERIODIC"     |       |boundary conditions|
|dir                        |string |"."            |       |directory for output files|
|myseed                     |int    |time(NULL)     |       |seed of random number generator|
|restart                    |bool   |false          |       |if true, will restart simulation from last timestep recorded|
|restart_time               |double |-1             |s      |time to restart simulation from|
|**ACTIN**                  |       |               |       | |
|nmonomer                   |double |11             |       |number of beads per filament|
|npolymer                   |double |3              |       |number of polymers in the network|
|actin_length               |double |0.5            |um     |Length of a single actin monomer|
|actin_pos_str              |string |               |       |Starting positions of actin polymers, commas delimit coordinates; semicolons delimit positions|
|link_length                |double |1              |       |Length of links connecting monomers|
|polymer_bending_modulus    |double |0.068          |pn*um^2|Bending modulus of a filament|
|fracture_force             |double |100000000      |pN     |filament breaking poiafines|
|link_stretching_stiffness  |double |1              |pN/um  |stiffness of link|
|**MOTORS**                 |       |               |       ||
|a_motor_density            |double |0.05           |um^(-2)|density of active motors |
|a_motor_pos_str            |string |               |       |Starting positions of motors, commas delimit coordinates; semicolons delimit positions|
|a_m_kon                    |double |1              |s^(-1) |active motor on rate|
|a_m_koff                   |double |0.1            |s^(-1) |active motor off rate|
|a_m_kend                   |double |0.1            |s^(-1) |active motor off rate at filament end|
|a_motor_stiffness          |double |1              |pN/um  |active motor spring stiffness|
|a_motor_length             |double |0.4            |um     |length of motor|
|a_m_stall                  |double |10             |pN     |stall force of motors|
|a_motor_v                  |double |1              |um/s   |velocity along filaments towards barbed end when attached|
|motor_intersect_flag       |boolean|false          |       |if true, then motors are placed at filament intersections|
|a_linkage_prob             |double |1              |       |probability that filaments are linked by a motor if motor_intersect_flag = true|
|dead_head_flag             |boolean|false          |       |if true, then head [dead_head] of all motors remains stationary throughout sim|
|dead_head                  |int    |0              |       |can be 0 or 1; head that remains stationary if dead_head_flag=true|
|**CROSSLINKS**             |       |               |       ||
|p_motor_density            |double |0.05           |um^(-2)|number of passive motors|
|p_motor_pos_str            |string |               |       |Starting positions of xlinks, commas delimit coordinates; semicolons delimit positions|
|p_m_kon                    |double |1              |s^(-1) |passive motor on rate|
|p_m_koff                   |double |0.1            |s^(-1) |passive motor off rate|
|p_m_kend                   |double |0.1            |s^(-1) |passive motor off rate at filament end|
|p_motor_stiffness          |double |1              |s^(-1) |xlink spring stiffness (pN/um)|
|p_motor_length             |double |0.15           |s^(-1) |length of xlink|
|p_m_stall                  |double |0              |pN     |stall force|
|link_intersect_flag        |boolean|false          |       |if true, then crosslinks are placed at filament intersections|
|p_linkage_prob             |double |1              |       |probability that filaments are crosslinked if link_intersect_flag = true|
|p_dead_head_flag           |boolean|false          |       |if true, then head [p_dead_head] of all xlinks remains stationary throughout sim|
|p_dead_head                |int    |0              |       |can be 0 or 1; head that remains stationary if p_dead_head_flag=true|
|static_cl_flag             |boolean|false          |       |flag to indicate compeletely static xlinks; i.e, no walking, no detachment|
|**SHEAR**                  |       |               |       ||
|strain_pct                 |double |0              |       |pre-strain (e.g., 0.5 means a strain of 0.5*xrange)|
|time_of_strain             |double |0              |       |time of pre-strain|
|d_strain_pct               |double |0              |s      |differential strain (e.g., 0.5 means a strain of 0.5*xrange)|
|time_of_dstrain            |double |10000          |s      |time when differential strain begins|
|diff_strain_flag           |boolean|false          |       |flag to use if differential strain should be linear (in one direction)|
|osc_strain_flag            |boolean|false          |       |flag to use if differential strain should be oscillatory (like Gardel, Science 2004)|
|n_bw_shear                 |int    |10^8           |s      |number of timesteps between subsequent differential strains |
|d_strain_freq              |double |1              |Hz     |frequency of differential oscillatory strain|


### Contribution guidelines ###

* Email: dinner@uchicago.edu

### Who do I talk to? ###

* Simon Freedman (simonfreedman@uchicago.edu)
* Aaron Dinner (dinner@uchicago.edu)
