% you need to download the SFS Toolbox: https://github.com/sfstoolbox/sfs and
% run SFS_start before the execution of this script

% configuration
conf.c = 343; % m/s, speed of sound
conf.resolution = 300; % points along every dimension => 300x300x300
conf.phase = 0; % to create a movie one can change the phase and recalculate the sound field for every frame
conf.usenormalisation = false;
conf.plot.useplot = false;
conf.showprogress = false;

% calculate the sound field of a point source with a frequency of 1000 Hz
f = 1000; % Hz
xs = [0 -0.1 0.5]; % m, position of point source
[P,x,y,z] = sound_field_mono_point_source([-2 2],[-2 2],[-2 2],xs,f,conf);

% TODO: we should think about how to store x,y,z axes

% save sound field data
save('-v7','point_source.mat','P');
