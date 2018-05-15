"""

Color-plot strategies for Player and Dealer.
Values close to 1 are plotted in stronger hues
of green, and values close to 0 are plotted in
stronger hues of red, as specified by the
cmap='RdYlGn' in the imshow() method.

"""

import pylab as plt
import csv
from mpl_toolkits.axes_grid1 import make_axes_locatable

# Download evolved strategies

strategy_player=[]
strategy_dealer=[]

with open('strategy_player.csv', 'r') as file:
    reader = csv.reader(file)
    strategy_player = list(reader)
    
with open('strategy_dealer.csv', 'r') as file:
    reader = csv.reader(file)
    strategy_dealer = list(reader)

strategy_player=strategy_player[0]
strategy_player=[float(x) for x in strategy_player]
sp=[]
for i in range(13):
    row=[]
    for j in range(13):
        row+=[strategy_player[13*i+j]]
    sp+=[row]

strategy_dealer=strategy_dealer[0]
strategy_dealer=[float(x) for x in strategy_dealer]
sd=[]
for i in range(13):
    row=[]
    for j in range(13):
        row+=[strategy_dealer[13*i+j]]
    sd+=[row]

# Plot Player's strategy

# ticks labels
ticks=['A','K','Q','J','T','9','8','7','6','5','4','3','2']
# position of the grid lines
ticks_range=[i-0.5 for i in range(13)]
# range of labels of ticks is shifted so that they appear in the middle
# between the grid lines
label_range=[i for i in range(13)]
#figure
fig,ax=plt.subplots(figsize=(5,5),dpi=80)
im = plt.imshow(sp, cmap='RdYlGn')
# remove ticks
plt.tick_params(bottom='off',top='off',left='off') 
# specify that x axis ticks are to be on top (default is on the bottom)
ax.xaxis.set_tick_params(labeltop='on', labelbottom='off')
# position the ticks labels
ax.xaxis.set(ticks=label_range, ticklabels=ticks)
ax.yaxis.set(ticks=label_range, ticklabels=ticks)
# position the grid; the grid will be minor ticks (the labels above are major)
ax.set_xticks(ticks_range, minor=True)
ax.set_yticks(ticks_range, minor=True)
ax.xaxis.grid(True, which='minor',color='black')
ax.yaxis.grid(True, which='minor',color='black')
# specify position of the colorbar
divider = make_axes_locatable(ax)
cax = divider.append_axes("bottom", size="5%", pad=0.05)
# position the colorbar
cbar=plt.colorbar(im, cax=cax, orientation='horizontal')
cbar.ax.tick_params(labelsize=8) 
# show and save the plot
plt.show()
fig.savefig("Flop_regret_player_strategy_B2_P2.pdf",\
                    dpi=80, facecolor='w', edgecolor='w',\
                    orientation='portrait', papertype=None, format=None,\
                    transparent=False, bbox_inches=None, pad_inches=0.1,\
                    frameon=None)


# Plot Dealer's strategy
    
# ticks labels
ticks=['A','K','Q','J','T','9','8','7','6','5','4','3','2']
# position of the grid lines
ticks_range=[i-0.5 for i in range(13)]
# range of labels of ticks is shifted so that they appear in the middle
# between the grid lines
label_range=[i for i in range(13)]
#figure
fig,ax=plt.subplots(figsize=(5,5),dpi=80)
im = plt.imshow(sd, cmap='RdYlGn')
# remove ticks
plt.tick_params(bottom='off',top='off',left='off') 
# specify that x axis ticks are to be on top (default is on the bottom)
ax.xaxis.set_tick_params(labeltop='on', labelbottom='off')
# position the ticks labels
ax.xaxis.set(ticks=label_range, ticklabels=ticks)
ax.yaxis.set(ticks=label_range, ticklabels=ticks)
# position the grid; the grid will be minor ticks (the labels above are major)
ax.set_xticks(ticks_range, minor=True)
ax.set_yticks(ticks_range, minor=True)
ax.xaxis.grid(True, which='minor',color='black')
ax.yaxis.grid(True, which='minor',color='black')
# specify position of the colorbar
divider = make_axes_locatable(ax)
cax = divider.append_axes("bottom", size="5%", pad=0.05)
# position the colorbar
cbar=plt.colorbar(im, cax=cax, orientation='horizontal')
cbar.ax.tick_params(labelsize=8) 
# show and save the plot
plt.show()
fig.savefig("Flop_regret_dealer_strategy_B2_P2.pdf",\
                    dpi=80, facecolor='w', edgecolor='w',\
                    orientation='portrait', papertype=None, format=None,\
                    transparent=False, bbox_inches=None, pad_inches=0.1,\
                    frameon=None)