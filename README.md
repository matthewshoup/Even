# EVEN
Second-harmonic beast audio plugin.

Rev 0.1:
- dedicated 2nd Harmonic control
- asymmetric tube-inspired nonlinear transfer
- PURE H2 centered square-law generator
- H2 SOLO
- Drive, Asymmetry, Bias, Warmth, Iron, Mix and Output
- stateful warmth and transformer-flux stages
- VST3, AU and Standalone universal macOS builds

Next: 4x/8x oversampling, H2 Lock adaptive biasing, FFT harmonic meter, calibrated triode/DHT personalities and automatic gain compensation.


## Hi-Fi topology
INPUT -> clean 6SN7 preamp -> dedicated H2 generator -> 12BH7 output follower -> iron -> OUTPUT.

The 12AX7 driver experiment was removed: EVEN is now deliberately optimized for a clean fundamental, strong controllable second harmonic, and reduced unnecessary higher-order distortion.
