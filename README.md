# PV-Anzeige
Resemblance to the official SunnyPortal plus additional WallBox information

The official SunnyPortal has a fine live visualization of the power streams in your house.
Its drawback is, you are kicked out after several minutes, after a given number of accesses to the database of SunnyPortal and some more restrictions.

To circumvent those restrictions I make my own visualization.

As an addition the power flowing to the wallbox is displayed as well. The information for that comes from Nico Treffkorn's [SmartCharger](http://www.eb-systeme.de/?page_id=1265), working together with some KEBA wall boxes.

There will be no statistics and no database, those are handled well by SunnyPortal.

Modelled with QT Creator, the basic display shows up as follows:

![grafik](https://user-images.githubusercontent.com/26298406/128635477-f4f7ed47-aaed-43f9-93fd-ab3bc7f4b9b8.png)

The pink texts and values are normally invisible and appear in white as all other text on mouse over.

The arrows indicate current flow. Only those arrows show up where current is actually flowing.

Depending on the current direction, some texts change, e.g. "Batterie-landung" / Batterie-entladung" or "Netzbezug" / "Netz-einspeisung".

If power from PV generator is insufficient to satisfy power consumption in the house, additional power is delivered by the battery or/and by the grid. This is shown as colored patches according to the percentage of power from both sources.

----

Below are shown some different power situations:

![grafik](https://user-images.githubusercontent.com/26298406/128636065-2350fa99-9545-4a0b-a25e-809d85646bc3.png)

Left: EV attached, not charging, house is supplied from PV, battery and grid.

Middle: No EV attached (0.04 kW is false display), house is supplied from PV, grid and battery.

Right: EV attached and charging, house gets power from PV and battery, power is delivered to the grid, too.

The displayed power levels do not depend from each other in this examples, the values are generated at random.
