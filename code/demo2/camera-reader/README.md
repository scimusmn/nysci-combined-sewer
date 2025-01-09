# camera-reader

This python application reads the addresses of blinking LEDs and allows you to query the results.

## installation

First, you need to install miniconda. Open PowerShell and type

```
curl https://repo.anaconda.com/miniconda/Miniconda3-latest-Windows-x86_64.exe -o miniconda.exe
Start-Process -FilePath ".\miniconda.exe" -ArgumentList "/S" -Wait
del miniconda.exe
```

Next, open a command prompt (i.e. `cmd.exe` - this WILL NOT work in PowerShell!) and navigate to this repo. Type

```
conda env create -f environment.yml
conda activate css-camera
```

You should now be able to successfully run `python cam.py`

## usage

A preview of the camera feed will be displayed. When you press any key, it will capture `FRAME_COUNT` images, delaying for `DELAY` between each capture. Finally, it will display the query window.

There are two sliders on the query window: `high` and `low`. Image brightnesses above the `high` value will be considered a 1, and brightnesses below the `low` value will be considered a zero. Any pixel with a brightness on any frame between 0 and 1 will be considered
non-signalling and colored red. I suggest adjusting the high and low sliders until most of the image is ignored aside from the area you are interested in; you may then click any pixel to see its value printed on the console.
