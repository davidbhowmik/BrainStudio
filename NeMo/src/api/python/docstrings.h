#define NETWORK_DOC "A Network is constructed by adding individual neurons and synapses to the\nnetwork. Neurons are given indices (from 0) which should be unique for each\nneuron. When adding synapses the source or target neurons need not\nnecessarily exist yet, but should be defined before the network is\nfinalised."
#define NETWORK_ADD_NEURON_TYPE_DOC "\n\nregister a new neuron type with the network\n\nInputs:\nname -- canonical name of the neuron type. The neuron type data is loaded from a plugin configuration file of the same name.\n\nReturns index of the the neuron type, to be used when adding neurons\n\nThis function must be called before neurons of the specified type can be\nadded to the network."
#define NETWORK_ADD_NEURON_DOC "\n\nadd one or more neurons to the network\n\nInputs:\ntype -- Neuron type\nidx -- Neuron index\nparameters... -- all neuron parameters\nstate... -- all state variables\n\nThe meaning of the parameters and state variables varies depending on the\nneuron typeThis function may be called either in a scalar or vector form.\nIn the scalar form all inputs are scalars. In the vector form, the neuron\nindex argument plus any number of the other arguments are lists of the same\nlength. In this second form scalar inputs are replicated the appropriate\nnumber of times"
#define NETWORK_ADD_SYNAPSE_DOC "\n\nadd a single synapse to the network\n\nInputs:\nsource -- Index of source neuron\ntarget -- Index of target neuron\ndelay -- Synapse conductance delay in milliseconds\nweight -- Synapse weights\nplastic -- Boolean specifying whether or not this synapse is plastic\n\nReturns Unique synapse ID\n\nThe input arguments can be any combination of lists of equal length and\nscalars. If the input arguments contain a mix of scalars and lists the\nscalar arguments are replicated the required number of times."
#define NETWORK_NEURON_COUNT_DOC "\n\n\n\nReturns number of neurons in the network"
#define NETWORK_CLEAR_NETWORK_DOC "\n\nclear all neurons/synapses from network"
#define CONFIGURATION_DOC "Global configuration"
#define CONFIGURATION_SET_CPU_BACKEND_DOC "\n\nspecify that the CPU backend should be used"
#define CONFIGURATION_SET_CUDA_BACKEND_DOC "\n\nspecify that the CUDA backend should be used\n\nInputs:\ndeviceNumber\n\nSpecify that the CUDA backend should be used and optionally specify a\ndesired device. If the (default) device value of -1 is used the backend\nwill choose the best available device. If the cuda backend (and the chosen\ndevice) cannot be used for whatever reason, an exception is raised. The\ndevice numbering is the numbering used internally by nemo (see\ncudaDeviceCount and cudaDeviceDescription). This device numbering may\ndiffer from the one provided by the CUDA driver directly, since NeMo\nignores any devices it cannot use."
#define CONFIGURATION_SET_STDP_FUNCTION_DOC "\n\nenable STDP and set the global STDP function\n\nInputs:\nprefire -- STDP function values for spikes arrival times before the postsynaptic firing, starting closest to the postsynaptic firing\npostfire -- STDP function values for spikes arrival times after the postsynaptic firing, starting closest to the postsynaptic firing\nminWeight -- Lowest (negative) weight beyond which inhibitory synapses are not potentiated\nmaxWeight -- Highest (positive) weight beyond which excitatory synapses are not potentiated\n\nThe STDP function is specified by providing the values sampled at integer\ncycles within the STDP window."
#define CONFIGURATION_BACKEND_DESCRIPTION_DOC "\n\nDescription of the currently selected simulation backend\n\nReturns Textual description of the currently selected backend\n\nThe backend can be changed using setCudaBackend or setCpuBackend"
#define CONFIGURATION_SET_WRITE_ONLY_SYNAPSES_DOC "\n\nSpecify that synapses will not be read back at run-time\n\nBy default synapse state can be read back at run-time. This may require\nsetting up data structures of considerable size before starting the\nsimulation. If the synapse state is not required at run-time, specify that\nsynapses are write-only in order to save memory and setup time. By default\nsynapses are readable"
#define CONFIGURATION_LOG_STDOUT_DOC "\n\nSwitch on logging to standard output"
#define CONFIGURATION_RESET_CONFIGURATION_DOC "\n\nReplace configuration with default configuration"
#define SIMULATION_DOC "A simulation is created from a network and a configuration object. The\nsimulation is run by stepping through it, providing stimulus as\nappropriate. It is possible to read back synapse data at run time. The\nsimulation also maintains a timer for both simulated time and wallclock\ntime."
#define SIMULATION_STEP_DOC "\n\nrun simulation for a single cycle (1ms)\n\nInputs:\nfstim -- An optional list of neurons, which will be forced to fire this cycle\nistim_nidx -- An optional list of neurons which will be given input current stimulus this cycle\nistim_current -- The corresponding list of current input\n\nReturns Neurons which fired this cycle"
#define SIMULATION_APPLY_STDP_DOC "\n\nupdate synapse weights using the accumulated STDP statistics\n\nInputs:\nreward -- Multiplier for the accumulated weight change"
#define SIMULATION_GET_MEMBRANE_POTENTIAL_DOC "\n\nget neuron membane potential\n\nInputs:\nidx -- neuron index\n\nReturns membrane potential\n\nThe neuron index may be either scalar or a list. The output has the same\nlength as the neuron input"
#define SIMULATION_ELAPSED_WALLCLOCK_DOC "\n\n\n\nReturns number of milliseconds of wall-clock time elapsed since first simulation step (or last timer reset)"
#define SIMULATION_ELAPSED_SIMULATION_DOC "\n\n\n\nReturns number of milliseconds of simulation time elapsed since first simulation step (or last timer reset)"
#define SIMULATION_RESET_TIMER_DOC "\n\nreset both wall-clock and simulation timer"
#define SIMULATION_CREATE_SIMULATION_DOC "\n\nInitialise simulation data\n\nInitialise simulation data, but do not start running. Call step to run\nsimulation. The initialisation step can be time-consuming."
#define SIMULATION_DESTROY_SIMULATION_DOC "\n\nStop simulation and free associated data\n\nThe simulation can have a significant amount of memory associated with it.\nCalling destroySimulation frees up this memory."
#define CONSTRUCTABLE_SET_NEURON_DOC "\n\nmodify one or more existing neurons\n\nInputs:\nidx -- Neuron index\nparameters... -- all neuron parameters\nstate... -- all state variables\n\nThe meaning of the parameters and state variables varies depending on the\nneuron type (specified when the neuron was created)This function may be\ncalled either in a scalar or vector form. In the scalar form all inputs are\nscalars. In the vector form, the neuron index argument plus any number of\nthe other arguments are lists of the same length. In this second form\nscalar inputs are replicated the appropriate number of times"
#define CONSTRUCTABLE_SET_NEURON_STATE_DOC "\n\nset neuron state variable\n\nInputs:\nidx -- neuron index\nvarno -- variable index\nval -- value of the relevant variable\n\nFor the Izhikevich model: 0=u, 1=v. The neuron and value parameters can be\neither both scalar or both lists of the same length"
#define CONSTRUCTABLE_SET_NEURON_PARAMETER_DOC "\n\nset neuron parameter\n\nInputs:\nidx -- neuron index\nvarno -- variable index\nval -- value of the neuron parameter\n\nThe neuron parameters do not change during simulation. For the Izhikevich\nmodel: 0=a, 1=b, 2=c, 3=d. The neuron and value parameters can be either\nboth scalar or both lists of the same length"
#define CONSTRUCTABLE_GET_NEURON_STATE_DOC "\n\nget neuron state variable\n\nInputs:\nidx -- neuron index\nvarno -- variable index\n\nReturns value of the relevant variable\n\nFor the Izhikevich model: 0=u, 1=v. The neuron index may be either scalar\nor a list. The output has the same length as the neuron input"
#define CONSTRUCTABLE_GET_NEURON_PARAMETER_DOC "\n\nget neuron parameter\n\nInputs:\nidx -- neuron index\nvarno -- variable index\n\nReturns value of the neuron parameter\n\nThe neuron parameters do not change during simulation. For the Izhikevich\nmodel: 0=a, 1=b, 2=c, 3=d. The neuron index may be either scalar or a list.\nThe output has the same length as the neuron input"
#define CONSTRUCTABLE_GET_SYNAPSES_FROM_DOC "\n\nreturn the synapse ids for all synapses with the given source neuron\n\nInputs:\nsource -- source neuron index\n\nReturns synapse ids\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
#define CONSTRUCTABLE_GET_SYNAPSE_SOURCE_DOC "\n\nreturn the source neuron of the specified synapse\n\nInputs:\nsynapse -- synapse id (as returned by addSynapse)\n\nReturns source neuron index\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
#define CONSTRUCTABLE_GET_SYNAPSE_TARGET_DOC "\n\nreturn the target of the specified synapse\n\nInputs:\nsynapse -- synapse id (as returned by addSynapse)\n\nReturns target neuron index\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
#define CONSTRUCTABLE_GET_SYNAPSE_DELAY_DOC "\n\nreturn the conduction delay for the specified synapse\n\nInputs:\nsynapse -- synapse id (as returned by addSynapse)\n\nReturns conduction delay of the specified synapse\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
#define CONSTRUCTABLE_GET_SYNAPSE_WEIGHT_DOC "\n\nreturn the weight for the specified synapse\n\nInputs:\nsynapse -- synapse id (as returned by addSynapse)\n\nReturns weight of the specified synapse\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
#define CONSTRUCTABLE_GET_SYNAPSE_PLASTIC_DOC "\n\nreturn the boolean plasticity status for the specified synapse\n\nInputs:\nsynapse -- synapse id (as returned by addSynapse)\n\nReturns plasticity status of the specified synapse\n\nThe input synapse indices may be either a scalar or a list. The return\nvalue has the same form"
