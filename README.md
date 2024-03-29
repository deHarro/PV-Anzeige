# PV-Anzeige
Resemblance to the official SunnyPortal plus additional WallBox information

The official SunnyPortal has a fine live visualization of the power streams in your house.
Its drawback is, you are kicked out after several minutes, after a given number of accesses to the database of SunnyPortal and some more restrictions.

To circumvent those restrictions I make my own visualization. See my [homepage](http://harald-sattler.de/html/pv-anzeige.htm) for additional info (in german - use translation feature at the bottom of the page).

As an addition the power flowing to the wallbox is displayed as well. The information for that comes from Nico Treffkorn's [SmartCharger](https://web.archive.org/web/20211018170240/www.eb-systeme.de/?page_id=1265) (original site is offline, link redirects to web.archive.org), working together with some KEBA wall boxes.

There will be no statistics and no database, those are handled well by SunnyPortal.

The code is based on _PowerNodeModel_ of Manuel and _mbmd_ of volkszaehler. 
Mbmd reads the PV converters over modbus, SmartCharger reads the battery converter and the SMA energy meter as well as the Keba P20 wallbox, PowerNodeModel models the connection between data layer and GUI. Thanks go to Nico (@skynet74), Manuel (@mincequi), Andreas (@andig) and Andreas (@DerAndereAndi).

Modelled with QT Creator, the basic display shows up as follows:

![grafik](https://user-images.githubusercontent.com/26298406/128635477-f4f7ed47-aaed-43f9-93fd-ab3bc7f4b9b8.png)

In the final version the pink texts and values will become white as all other text.

This is, how the current implementation looks like:

![grafik](https://user-images.githubusercontent.com/26298406/150100975-b410f042-3a54-4f92-8d6e-6fdaa3609e60.png)

This is a screen shot of the live view, the system is running and data are actual values. I added some more readings the system delivers anyway and made the icons of car and scooter resemble the plug state of the wallbox.

The arrows indicate current flow. Only those arrows show up where current is actually flowing.

Depending on the current direction, some texts change, e.g. "Batterie-ladung" / Batterie-entladung" or "Netzbezug" / "Netz-einspeisung".

If power from PV generator is insufficient to satisfy power consumption in the house, additional power is delivered by the battery or/and by the grid. This is shown as colored patches according to the percentage of power from both sources. The rightmost picture below visualizes this situation.

----

Below are shown some possible power situations:

![grafik](https://user-images.githubusercontent.com/26298406/128638561-4f10fbc1-89bf-4901-b629-d7d414df77b6.png)

Left: EV attached, not charging (dark blue), PV generator delivers power to house, battery and grid.

Middle: EV attached and charging (light blue), PV generator delivers power to house, battery, grid and wallbox.

Right: No EV attached, house is supplied by PV, grid and battery.

The displayed power levels do not depend from each other in this examples, the values are generated randomly.
