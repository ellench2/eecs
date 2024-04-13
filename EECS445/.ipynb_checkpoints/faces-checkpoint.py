
"""
EECS 445 - Introduction to Machine Learning
Winter 2023 - Homework 3
PCA
"""

from sklearn.decomposition import PCA
import matplotlib.pyplot as plt
from pca import get_images, get_nn, get_image_from_filename

if __name__ == "__main__":
    # Part 2: PCA on faces
    images, pathdict = get_images()
    N, W, H = images.shape
    images = images.reshape(N, -1)  # flatten

    # TODO: run PCA on the images with 100 components
    face_pca = PCA(n_components=100, random_state=42)
    face_pca.fit(images)
    
    fig, axes = plt.subplots(
        3,
        8,
        figsize=(9, 4),
        subplot_kw={"xticks": [], "yticks": []},
        gridspec_kw=dict(hspace=0.1, wspace=0.1),
    )
    for i, ax in enumerate(axes.flat):
        ax.imshow((face_pca.components_[i]).reshape(64, 64), cmap="gray")
    plt.savefig("components.pdf")

    # TODO: get the lower dimensional representation
    # image_vectors = ???
    image_vectors = face_pca.transform(images)
    
    
    # find nearest neighbor for Amelie Mauresmo
    vec = get_image_from_filename(
        "Amelie_Mauresmo_0001.jpg", image_vectors, pathdict
    )
    nn, _, sim = get_nn(vec, image_vectors)

    fig, axes = plt.subplots(
        2,
        3,
        figsize=(9, 4),
        subplot_kw={"xticks": [], "yticks": []},
        gridspec_kw=dict(hspace=0.1, wspace=0.1),
    )
    for i, ax in enumerate(axes.flat):
        ax.imshow((images[nn[i]]).reshape(64, 64), cmap="gray")
    plt.savefig("amelie.jpg")
    
    vec_again = get_image_from_filename(
        "Allison_Janney_0002.jpg", image_vectors, pathdict
    )
    nn_1, _1, sim_1 = get_nn(vec_again, image_vectors)
    
    fig, axes = plt.subplots(
        2,
        3,
        figsize=(9,4),
        subplot_kw = {"xticks": [], "yticks": []},
        gridspec_kw=dict(hspace=0.1, wspace=0.1),
    )
    for i, ax in enumerate(axes.flat):
        ax.imshow((images[nn_1[i]]).reshape(64,64), cmap="gray")
        plt.savefig("allison.jpg")

