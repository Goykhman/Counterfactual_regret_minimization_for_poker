'''
  Prints player's and dealer's fit score time series.
'''


from __future__ import print_function
import csv
import matplotlib.pyplot as plt
import numpy as np

###########################################################################

# Evolution fit time series

time_series_player=[]
time_series_dealer=[]

with open('player_fit_time_series.csv', 'r') as file:
    reader = csv.reader(file)
    time_series_player = list(reader)
    
with open('dealer_fit_time_series.csv', 'r') as file:
    reader = csv.reader(file)
    time_series_dealer = list(reader)

time_series_player=time_series_player[0]
time_series_player=[float(x) for x in time_series_player]

# Saturation value for the Player
mn_player=np.mean(time_series_player[900:1000])
mn_player=int(10000*mn_player)/10000.0

time_series_dealer=time_series_dealer[0]
time_series_dealer=[float(x) for x in time_series_dealer]

# Saturation value for the Dealer
mn_dealer=np.mean(time_series_dealer[900:1000])
mn_dealer=int(10000*mn_dealer)/10000.0

time_series_plot=plt.figure(dpi=80)
plt.plot(range(len(time_series_player)),time_series_player,label="Player")
#plt.plot(range(len(time_series_dealer)),time_series_dealer,label="Dealer")
plt.xlabel("rounds of optimization")
plt.ylabel("return")
plt.title("Optimized strategy fit saturates to {} for Player".format(mn_player))
#plt.legend(loc='upper center')
ax = time_series_plot.gca()
ax.set_yticks(np.arange(1.13, 1.17, 0.01))
plt.grid()
plt.show()
time_series_plot.savefig("optimized_player_fit_B2_P2.pdf",\
                    dpi=80, facecolor='w', edgecolor='w',\
                    orientation='portrait', papertype=None, format=None,\
                    transparent=False, bbox_inches=None, pad_inches=0.1,\
                    frameon=None)

time_series_plot=plt.figure(dpi=80)
plt.plot(range(len(time_series_player)),time_series_dealer,label="Player")
#plt.plot(range(len(time_series_dealer)),time_series_dealer,label="Dealer")
plt.xlabel("rounds of optimization")
plt.ylabel("return")
plt.title("Optimized strategy fit saturates to {} for Dealer".format(mn_dealer))
#plt.legend(loc='upper center')
ax = time_series_plot.gca()
ax.set_yticks(np.arange(0.83, 0.87, 0.01))
plt.grid()
plt.show()
time_series_plot.savefig("optimized_dealer_fit_B2_P2.pdf",\
                    dpi=80, facecolor='w', edgecolor='w',\
                    orientation='portrait', papertype=None, format=None,\
                    transparent=False, bbox_inches=None, pad_inches=0.1,\
                    frameon=None)