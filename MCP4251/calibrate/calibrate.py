import numpy as np

# Use some random values between 0-511 using setValue() and read off the actual resistance using a multimeter
# Put those measures into the table in the format [random number, resistance in kOhm] 
measures = np.array([
    [0, 4.96],
    [67, 3.69],
    [128, 2.53],
    [234, 0.51],
    [5, 4.87],
    [199, 1.18],
    [46, 4.09],
    [172, 1.69],
    [152, 2.07],
    [105, 2.97],
    [58, 3.87],
    [240, 0.4],
    [133, 2.44],
    [60, 3.82],
    [210, 0.97],
    [33, 4.34],
    [119, 2.7],
    [87, 3.31],
    [201, 1.14],
    [142, 2.27],
    [54, 3.94],
    [249, 0.23],
    [233, 0.53],
    [223, 0.72],
    [121, 2.66],
    [13, 4.72]
])

X = measures[:, 1]
y = measures[:, 0]

# Extend X by ones in the first column
X = np.c_[np.ones_like(X), X]

theta = np.linalg.inv(X.T.dot(X)).dot(X.T).dot(y)

y_pred = X.dot(theta)

print("// R²:", 1 - np.sum((y - y_pred) ** 2) / np.sum((y - y.mean()) ** 2)) # Optimum is 1
print("#define MCP4251_CALIBRATE_A", theta[0])
print("#define MCP4251_CALIBRATE_B", -theta[1])
