clear all;
close all;
clc;
% SPEED SENSOR PIN 18 (RB8)
% LOAD CELL PIN 2 (RA0)
%% SERIAL COMMUNICATION ESTABLISHMENT AND VARIABLE DECLARATION
serial3 = serial('COM3');
set(serial3,'BaudRate',9600,'Parity','none','terminator',000,'DataBits',8);
fopen(serial3);

RPM = 0;
time = now;
freq = 0;
torque = 0;
clocked = 0;
duration = 0;

%% REAL TIME DATA COLLECTION STAGE 
% (this section requires serial communication previously established)

% This MATLAB script generates a real time plot of voltage values collected
% from an Agilent(TM) 34401A Digital Multimeter over the serial
% port(RS232). The data is collected and plotted until the predefined stop
% time is reached. This example also demonstrates automating a query based
% interaction with an instrument while monitoring the results live.
%
% This script can be modified to be used on any platform by changing the
% serialPort variable.
% 
% Example:-
%
% On Linux:     serialPort = '/dev/ttyS0';
%
% On MacOS:     serialPort = '/dev/tty.KeySerial1';
%
% On Windows:   serialPort = 'COM1';
%
%
% The script may also be updated to use any instrument/device to collect
% real time data. You may need to update the SCPI commands based on
% information in the instrument's programming manual.
%
% To generate a report of this entire script, you may use the PUBLISH
% command at the MATLAB(R) command line as follows: 
% 
% publish(real_time_data_plot);
% 
% Author: Ankit Desai
% Copyright 2009 - The MathWorks, Inc.

% Set up the figure window

figure(2);

whitebg('black');
set(gcf, 'color', 'black');


[XY,H1,H2] = plotyy(time, RPM, time, torque);
% XY handles the axes and H1, H2 handle the curve
% XY(1) hadles the left line axes and H1 handles the left curve
% XY(2) hadles the right line axes and H2 handles the right curve

title('DYNAMOMETER','Color', 'c')
xlabel('Time','Color', 'c')

% set the left (RPM) y-axis label and label color
ylabel(XY(1),'Engine Speed (RPM)','Color', 'g')
% set the left (Torque) y-axis label and label color
ylabel(XY(2),'Torque (ft. lbs.)','Color', 'm')

% set the left (RPM) y-axis line thickness and curve color
set(H1,'LineWidth',2, 'color', 'green')
% set the right (Torque) y-axis line thickness and curve color
set(H2,'LineWidth',2, 'color','m')

set(XY(1), 'Ylim', [0 4500], 'YTick', [0:500:4500]); 
% set the left (RPM) y-axis range and tick; 
set(XY(1),'YMinorGrid','on')
% set the left (RPM) y-axis color
set(XY(1),'ycolor','g')
% set the top y-axis color
set(XY(1),'Xcolor','c')

% set the right y-axis range and tick; disable x-axis for the right y-axis
% to avoid x-axis duplication.
set(XY(2), 'Ylim', [0 1100], 'YTick', [0:100:1100], 'XTick', []); 

% set the right (Torque) y axis grid
set(XY(2),'YMinorGrid','on') 
% set the x-axis grid
set(XY(2),'XMinorGrid','on') 
% set the right (Torque) y-axis color
set(XY(2),'ycolor','m') 
% set the bottom (Time) x-axis color
set(XY(2),'Xcolor','c')
set(XY(2),'XGrid','on')
hold on;

% link the x-axis of the right y-axis (RPM) and left y-axis (Torque) curves
% to synchronize the curves timing.
linkaxes(XY,'x');

datetick('x','mm/DD HH:MM');

% figure(3)
% plot(duration)






% Set the time span and interval for data collection
timeInterval = 0.005;

% Collect data
count = 1;

% Set up the time to run the program for
prompt = 'How long do you want it to be run for (in seconds)?: ';
set_time = input(prompt);

tic;

while (toc < set_time)
    
    time(count) = datenum(clock); 
    
    fprintf(serial3, 't');
    ticks = fscanf(serial3,'%f');
    %freq(count)= (20048/ticks)*50*16; 
    freq(count) = 1001856/ticks ;
%    RPM(count) = freq(count) * 60 / 4;
    RPM(count) = freq(count) * 60 / 6;
     
    fprintf(serial3, 'T');
    ticks = fscanf(serial3,'%f'); 
    
    torque(count)= ticks;
    
    clocked(count) = toc;
    
    if count > 1
    duration(count) = clocked(count) - clocked(count-1);
    end
    
 %    set(plotHandle,'YData',RPM,'XData',time);
 %    set(figureHandle,'Visible','on');
    plotyy(time, RPM, time, torque);
  %  datetick('x','mm/DD HH:MM');
    hold on;

   
    
    %pause(timeInterval);
    count = count +1;

end

%% THE RPM AND TORQUE CURVES ARE PLOTTED
figure(2);

whitebg('black');
set(gcf, 'color', 'black');


[XY,H1,H2] = plotyy(time, RPM, time, torque);
% XY handles the axes and H1, H2 handle the curve
% XY(1) hadles the left line axes and H1 handles the left curve
% XY(2) hadles the right line axes and H2 handles the right curve

title('DYNAMOMETER','Color', 'c')
xlabel('Time','Color', 'c')

% set the left (RPM) y-axis label and label color
ylabel(XY(1),'Engine Speed (RPM)','Color', 'g')
% set the left (Torque) y-axis label and label color
ylabel(XY(2),'Torque (ft. lbs.)','Color', 'm')

% set the left (RPM) y-axis line thickness and curve color
set(H1,'LineWidth',2, 'color', 'green')
% set the right (Torque) y-axis line thickness and curve color
set(H2,'LineWidth',2, 'color','m')

set(XY(1), 'Ylim', [0 4500], 'YTick', [0:500:4500]); 
% set the left (RPM) y-axis range and tick; 
set(XY(1),'YMinorGrid','on')
% set the left (RPM) y-axis color
set(XY(1),'ycolor','g')
% set the top y-axis color
set(XY(1),'Xcolor','c')

% set the right y-axis range and tick; disable x-axis for the right y-axis
% to avoid x-axis duplication.
set(XY(2), 'Ylim', [0 1100], 'YTick', [0:100:1100], 'XTick', []); 

% set the right (Torque) y axis grid
set(XY(2),'YMinorGrid','on') 
% set the x-axis grid
set(XY(2),'XMinorGrid','on') 
% set the right (Torque) y-axis color
set(XY(2),'ycolor','m') 
% set the bottom (Time) x-axis color
set(XY(2),'Xcolor','c')
set(XY(2),'XGrid','on')

% link the x-axis of the right y-axis (RPM) and left y-axis (Torque) curves
% to synchronize the curves timing.
linkaxes(XY,'x');

datetick('x','mm/DD HH:MM');

% figure(3)
% plot(duration)

%% Let's save the acquired data as an excel file

header = {'RPM' 'Torque' 'Time'};

display ('For the question below, use Y or y to reply positively');
display ('other inputs will be recongized as a negative reply.');

answer = input('Would you like the data to be saved?(y/n):', 's');


if answer == 'y' || answer == 'Y'
    
    % Create a name for the excel file
    prompt = 'Type in the name to be used for the Excel file: ';

    % Create string type variable
    str = input(prompt,'s');

    % Concatenate the inputted string with the suffix .xlsx
    str_con = strcat(str, '.xlsx');

    % Concatenate RPM, torque and time arrays into a matrix
    run_data = [transpose(RPM), transpose(torque), transpose(clocked)];

    % Save the file
    % xlswrite(str_con,run_data);
    warning('off','MATLAB:xlswrite:AddSheet');
    xlswrite(str_con, header, 'data_collected') 
    xlswrite(str_con, run_data, 'data_collected','A2') 
end

%% Clean up the serial object
fclose(serial3);
delete(serial3);
clear serail3;