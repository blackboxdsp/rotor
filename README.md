# Rotor
  
> Modern ring modulation
  
<div style="text-align: center">
    <img src="./screenshot.png" alt="Rotor screenshot" title="Rotor running in Ableton Live 10" height="720"/>
</div>
  
Rotor is a variable waveform ring modulation plugin targeting VST3 and AU for OS X and Windows platforms. It uses wavetable synthesis to generate various simple waveforms that act as the modulation signal for the input. It is compiled using the [JUCE](https://juce.com/) framework for C++.

## Dependencies

- JUCE (v5.4.6)
- VST3 SDK (v3.6.14)

## Usage

Rotor is capable of running within any plugin host as long as it supports VST3 or AU. Please refer to your DAW's manual to see compatible plugin formats.

### Parameters

- __Shape__: Selects the waveform to use as the modulation signal. Included are sine, triangle, sawtooth, square, and noise waveforms. The plugin defaults to a sine wave.

- __Rate__: Changes the frequency of the modulation signal within the range of 10Hz to 12kHz. The default rate is 500Hz.

- __Noise__: Control amplitude of random noise from 0.0 to 1.0 (basically the noise's gain multiplier). The default value is 0.0, which is zero noise.

- __Pulse Width__: Determines the interval between the rise and fall of a pulse, most commonly associated with square waves. The default value is 0.5 resulting in an even pulsation. _This parameter only affects the square wave._

- __Invert__: Sets the inversion of the modulation signal, which can sometimes lead to interesting phase cancellations with the original input. The default is a non-inverted modulation.

- __Mix__: Adjusts the output signal's ratio between dry and wet. The default value is 50%.

- __Level__: Controls the final output level of the plugin. The default is set to 0.0dB.

## Contributing

Should you discover any bugs or undesirable behavior, please feel free to open an issue or submit a pull request. I am happy to accept them as long as they don't alter product direction, otherwise I am completely open to discuss any ideas that you may have regarding Rotor.

### Building

Rotor is built using the JUCE framework, which graciously provides project files for various IDEs.

For Windows users, simply navigate to `Builds/VisualStudio2019/Rotor.sln` then build either of the two available configurations. Similarly, OS X users may navigate to `Builds/MACOSX/Rotor.xcodeproj` and build using XCode.

If you have any issues building the plugin, please contact me or file an issue.

## License

Copyright (C) 2020 Matthew Maxwell

This program is free software: you can redistribute it and / or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTIBILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details. 

You should have received a copy of the GNU General Public License along with this program. If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).

