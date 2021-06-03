# PV-Anzeige
Resemblance to the official SunnyPortal + WallBox

The official SunnyPortal has a fine live visualization of the power streams in your house.
Its drawback is, you are kicked out after several minutes, after a given number of accesses to the database of SunnyPortal and some more restrictions.

To circumvent those restrictions I make my own visualization.

As an addition the power flowing to the wallbox is displayed as well. The information for that comes from Nico Treffkorn's SmartCharger, working together with some KEBA wall boxes.

In the end it should show up similar to this Excel prototype

![grafik](https://user-images.githubusercontent.com/26298406/120485830-c3b6d180-c3b4-11eb-9080-f2ae95a3c6ef.png)


There will be no statistics and no database, those are handled well by SunnyPortal.

Modelled with QT Creator, the basic display shows up as follows:

![grafik](https://user-images.githubusercontent.com/26298406/120667906-3b0c6400-c48e-11eb-9db1-e97a7d9adbd8.png)


The pink values are normally invisible and appear on mouse over (to be defined if mouse over the window or any of the icons or ...). If visible, the characters are white as all others.
