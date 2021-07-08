# PV-Anzeige
Resemblance to the official SunnyPortal plus additional WallBox information

The official SunnyPortal has a fine live visualization of the power streams in your house.
Its drawback is, you are kicked out after several minutes, after a given number of accesses to the database of SunnyPortal and some more restrictions.

To circumvent those restrictions I make my own visualization.

As an addition the power flowing to the wallbox is displayed as well. The information for that comes from Nico Treffkorn's [SmartCharger](http://www.eb-systeme.de/?page_id=1265), working together with some KEBA wall boxes.

There will be no statistics and no database, those are handled well by SunnyPortal.

Modelled with QT Creator, the basic display shows up as follows:

![Darstellung_PV-Anzeige](https://user-images.githubusercontent.com/26298406/124920880-17da5480-dff8-11eb-824f-2bed3e3577b1.png)


The pink values are normally invisible and appear on mouse over in white, as all other text.
The arrows are normally invisible and only those appear, where current is actually flowing.
