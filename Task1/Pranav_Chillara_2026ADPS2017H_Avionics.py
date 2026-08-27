import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from scipy.signal import savgol_filter


FILE_NAME = "Depth Data.csv"

df = pd.read_csv(FILE_NAME)

# Clean depth values
df["Depth (m)"] = pd.to_numeric(df["Depth (m)"], errors="coerce")
df["Time (s)"] = df["Point"] - 1

depth_values = df["Depth (m)"].copy()


# Remove unusual readings using a local median
local_median = depth_values.rolling(
    window=11,
    center=True,
    min_periods=1
).median()

difference = (depth_values - local_median).abs()

local_mad = difference.rolling(
    window=11,
    center=True,
    min_periods=1
).median()

local_mad = local_mad.replace(0, np.nan)

is_outlier = difference > (6 * local_mad)

depth_values[is_outlier] = np.nan
depth_values = depth_values.interpolate(
    method="linear",
    limit_direction="both"
)

# Smooth the cleaned data
filter_window = 11
poly_order = 2

if len(depth_values) < filter_window:
    filter_window = len(depth_values)

    if filter_window % 2 == 0:
        filter_window -= 1

if filter_window > poly_order:
    filtered_depth = savgol_filter(
        depth_values.to_numpy(),
        window_length=filter_window,
        polyorder=poly_order
    )
else:
    filtered_depth = depth_values.to_numpy()

print("====================================")
print(" DEPTH SENSOR ANALYSIS")
print("====================================")
print(f"Number of samples: {len(df)}")
print(f"Recording duration: {df['Time (s)'].iloc[-1]} seconds")
print(f"Outlier readings removed: {is_outlier.sum()}")
print(f"Minimum depth: {depth_values.min():.2f} m")
print(f"Maximum depth: {depth_values.max():.2f} m")
print("====================================")

fig, ax = plt.subplots(figsize=(12, 6))

min_y = filtered_depth.min() - 30
max_y = filtered_depth.max() + 30

ax.set_xlim(0, df["Time (s)"].iloc[-1])
ax.set_ylim(min_y, max_y)

ax.set_title(
    "Ship Depth Sensor — Real-Time Monitoring",
    fontsize=16,
    fontweight="bold"
)

ax.set_xlabel("Time (seconds)")
ax.set_ylabel("Depth (m)")
ax.grid(True, alpha=0.3)

filtered_line, = ax.plot(
    [],
    [],
    linewidth=2,
    label="Filtered Depth"
)

raw_line, = ax.plot(
    [],
    [],
    alpha=0.25,
    linewidth=1,
    label="Raw Sensor Data"
)

ax.legend()

def init():
    filtered_line.set_data([], [])
    raw_line.set_data([], [])
    return filtered_line, raw_line

def update(frame):
    time_values = df["Time (s)"].iloc[:frame + 1]
    raw_depth = df["Depth (m)"].iloc[:frame + 1]
    smooth_depth = filtered_depth[:frame + 1]

    raw_line.set_data(time_values, raw_depth)
    filtered_line.set_data(time_values, smooth_depth)

    current_time = df["Time (s)"].iloc[frame]

    ax.set_title(
        f"Ship Depth Sensor — Time: {current_time} s",
        fontsize=16,
        fontweight="bold"
    )

    return filtered_line, raw_line

animation = FuncAnimation(
    fig,
    update,
    frames=len(df),
    init_func=init,
    interval=1000,
    blit=False,
    repeat=False
)
plt.tight_layout()
plt.show()
