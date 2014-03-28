% you need to download the SFS Toolbox: https://github.com/sfstoolbox/sfs and
% run SFS_start before the execution of this script
clear all
close all
% configuration
conf.c = 343; % m/s, speed of sound
conf.resolution = 128; % points along every dimension => 128x128x128
conf.phase = 0; % to create a movie one can change the phase and recalculate the sound field for every frame
conf.usenormalisation = false;
conf.plot.useplot = false;
conf.showprogress = false;
% loudspeaker array
conf.secondary_sources.number = 64;
conf.secondary_sources.size = 3; % / m
conf.secondary_sources.center = [0 0 0]; % / m
conf.secondary_sources.geometry = 'circle';
conf.secondary_sources.x0 = [];
% WFS settings
conf.usetapwin = true;
conf.tapwinlen = 0.3;
conf.xref = [0 0 0];
conf.dimension = '2.5D';
conf.driving_functions = 'default';

% calculate the sound field of a point source with a frequency of 1000 Hz
f = 10000; % Hz
xs = [0 2.5 0]; % m, position of point source
src = 'ps'; % source type


N = 10;
for i=1:N

conf.phase = (i/N)*2*pi; % to create a movie one can change the phase and recalculate the sound field for every frame

[P,x,y,z] = sound_field_mono_wfs([-2 2],[-2 2],[-2 2],xs,src,f,conf);

P = smooth3(P); %antialias high spatial frequency components

P_single_real = single(real(P)); %save real component as single 32bit value

save('-v6',strcat('wfs_point_source_128_single_real', num2str(i),'.mat'),'P_single_real');

end
