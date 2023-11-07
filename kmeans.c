#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ITERATIONS 100

// Data point structure
typedef struct {
    double x;
    double y;
} Point;

// Cluster structure
typedef struct {
    Point centroid;
    int num_points;
} Cluster;

// Function to calculate the Euclidean distance between two points
double distance(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Function to initialize cluster centroids randomly
void initialize_clusters(Point data[], int n, Cluster clusters[], int k) {
    for (int i = 0; i < k; i++) {
        int random_index = rand() % n;
        clusters[i].centroid = data[random_index];
        clusters[i].num_points = 0;
    }
}

// Function to assign each point to the nearest cluster
void assign_to_clusters(Point data[], int n, Cluster clusters[], int k) {
    for (int i = 0; i < n; i++) {
        double min_dist = distance(data[i], clusters[0].centroid);
        int min_index = 0;

        for (int j = 1; j < k; j++) {
            double dist = distance(data[i], clusters[j].centroid);
            if (dist < min_dist) {
                min_dist = dist;
                min_index = j;
            }
        }

        clusters[min_index].num_points++;
    }
}

// Function to update the cluster centroids
void update_clusters(Point data[], int n, Cluster clusters[], int k) {
    for (int i = 0; i < k; i++) {
        if (clusters[i].num_points == 0) {
            continue; // Avoid division by zero
        }

        double sum_x = 0.0;
        double sum_y = 0.0;

        for (int j = 0; j < n; j++) {
            if (i == 0 || distance(data[j], clusters[i].centroid) <
                              distance(data[j], clusters[i - 1].centroid)) {
                sum_x += data[j].x;
                sum_y += data[j].y;
            }
        }

        clusters[i].centroid.x = sum_x / clusters[i].num_points;
        clusters[i].centroid.y = sum_y / clusters[i].num_points;
    }
}

// Function to check if the clustering has converged
int has_converged(Cluster clusters[], Cluster old_clusters[], int k,
                  double tolerance) {
    for (int i = 0; i < k; i++) {
        if (distance(clusters[i].centroid, old_clusters[i].centroid) >
            tolerance) {
            return 0; // Not converged
        }
    }
    return 1; // Converged
}

int main() {
    int n = 100;               // Number of data points
    int k = 3;                 // Number of clusters
    double tolerance = 0.0001; // Tolerance for convergence

    Point data[n];
    Cluster clusters[k];
    Cluster old_clusters[k];

    // Initialize data points with random values
    for (int i = 0; i < n; i++) {
        data[i].x = (double)(rand() % 100);
        data[i].y = (double)(rand() % 100);
    }

    // Initialize cluster centroids randomly
    initialize_clusters(data, n, clusters, k);

    int iteration = 0;

    while (iteration < MAX_ITERATIONS) {
        // Save old cluster centroids
        for (int i = 0; i < k; i++) {
            old_clusters[i] = clusters[i];
            clusters[i].num_points =
                0; // Reset the number of points in each cluster
        }

        // Assign each point to the nearest cluster
        assign_to_clusters(data, n, clusters, k);

        // Update cluster centroids
        update_clusters(data, n, clusters, k);

        // Check for convergence
        if (has_converged(clusters, old_clusters, k, tolerance)) {
            break;
        }

        iteration++;
    }

    // Print the final cluster centroids
    printf("Final cluster centroids:\n");
    for (int i = 0; i < k; i++) {
        printf("Cluster %d: (%.2f, %.2f)\n", i + 1, clusters[i].centroid.x,
               clusters[i].centroid.y);
    }

    return 0;
}
