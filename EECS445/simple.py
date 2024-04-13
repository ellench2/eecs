
"""
EECS 445 - Introduction to Machine Learning
Winter 2023 - Homework 3
PCA
"""

from pca import generate_data, pca, plot

if __name__ == "__main__":
    # Part 1: PCA on a simple dataset
    data = generate_data()
    var, vec = pca(data)
    plot(data, (var, vec))
