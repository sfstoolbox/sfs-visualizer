% you need to download the SFS Toolbox: https://github.com/sfstoolbox/sfs and
% run SFS_start before the execution of this script

% configuration
conf = SFS_config_example;
conf.c = 343; % m/s, speed of sound
conf.resolution = 300; % points along every dimension => 300x300x300
conf.phase = 0; % to create a movie one can change the phase and recalculate the sound field for every frame
conf.showprogress = false;
% normalize sound field to 1 at position xref
conf.usenormalisation = true;
conf.xref = [0 1 0];

% calculate the sound field of a point source with a frequency of 1000 Hz
f = 1000; % Hz
xs = [0 -0.1 0.5]; % m, position of point source
% choose only a plane by setting one of the X,Y,Z value to a single number
sound_field_mono_point_source([-2 2],[-2 2],0,xs,f,conf); % x-y-plane
sound_field_mono_point_source([-2 2],0,[-2 2],xs,f,conf); % x-z-plane
sound_field_mono_point_source(0,[-2 2],[-2 2],xs,f,conf); % y-z-plane
