% you need to download the SFS Toolbox: https://github.com/sfstoolbox/sfs and
% run SFS_start before the execution of this script

% configuration
conf.c = 343; % m/s, speed of sound
conf.resolution = 300; % points along every dimension => 300x300x300
conf.phase = 0; % to create a movie one can change the phase and recalculate the sound field for every frame
conf.usenormalisation = false;
conf.plot.useplot = false;
conf.showprogress = false;
% loudspeaker array
conf.secondary_sources.number = 450;
conf.secondary_sources.size = 3; % / m
conf.secondary_sources.center = [0 0 0]; % / m
conf.secondary_sources.geometry = 'sphere';
conf.secondary_sources.x0 = [];
conf.secondary_sources.grid = 'gauss';
% WFS settings
conf.usetapwin = true;
conf.tapwinlen = 0.3;
conf.xref = [0 0 0];
conf.dimension = '3D';
conf.driving_functions = 'default';

% calculate the sound field of a point source with a frequency of 1000 Hz
f = 1000; % Hz
xs = [0 2.5 1]; % m, position of point source
src = 'ps'; % source type
[P,x,y,z] = sound_field_mono_wfs([-2 2],[-2 2],[-2 2],xs,src,f,conf);

% TODO: we should think about how to store x,y,z axes

% save sound field data
save('-v7','wfs3d_point_source.mat','P');
