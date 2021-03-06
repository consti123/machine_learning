import numpy as np
import h5py

# importing the adapted mnist data
def import_data_mnist():
	test_path     = "../../data/mnist/small/test.h5"
	training_path = "../../data/mnist/small/train.h5"
	
	f = h5py.File(training_path, 'r')
	images_train = f["images"].value
	labels_train = f["labels"].value
	f.close()
	
	f = h5py.File(test_path, 'r')
	images_test = f["images"].value
	labels_test = f["labels"].value
	f.close()
	
	print "images_train shape:"
	print np.shape(images_train)
	print "labels_train shape:"
	print np.shape(labels_train)
	print "images_test shape:"
	print np.shape(images_test)
	print "labels_test shape:"
	print np.shape(labels_test)

	return images_train, labels_train, images_test, labels_test

# filter data, target and images for the labels specified in labels_filter
# values in labels_filter need to be between 0 and 9
def filter_labels(data, target, labels_filter):
	data_append = []
	target_append = []
	for label in labels_filter:
		mask = np.where(target == label)
		data_label = data[mask]
		target_label = target[mask]
		data_append.append(data_label)
		target_append.append(target_label)
	data_filtered = np.concatenate([x for x in data_append])
	target_filtered = np.concatenate([x for x in target_append])
	return data_filtered, target_filtered


# function for the dim-reduction 
# use the two hottest pixels for now
def reduce_dimension(data):
	len_data = data.shape[0]
	data_return = np.zeros( (len_data,2) )
	# pixel (6,4) looks most important in the diff plot
	# pixel (6,7) looks second most important
	for i in range(len_data):
		data_return[i][0] = data[i][6,4]
		data_return[i][1] = data[i][6,7]
	return data_return

if __name__ == '__main__':
	# read in the data
	images_train, labels_train, images_test, labels_test = import_data_mnist()
	
	# filter data for 3s and 8s
	images_train, labels_train = filter_labels(images_train, labels_train, (3,8))
	print images_train.shape[0], "train-instances of 3s and 8s in the dataset were read in."
	images_test, labels_test = filter_labels(images_test, labels_test, (3,8))
	print images_test.shape[0], "test-instances of 3s and 8s in the dataset were read in."
	
	# reduce dimensions
	reduced_train = reduce_dimension(images_train)
	reduced_test  = reduce_dimension(images_test)

	# reshape the image data
	images_train = images_train.reshape(images_train.shape[0],images_train.shape[1]*images_train.shape[2])
	print "Train data reshaped to", images_train.shape
	images_test = images_test.reshape(images_test.shape[0],images_test.shape[1]*images_test.shape[2])
	print "Test data reshaped to", images_test.shape

	# change labels 3 -> 0, 8 -> 1
	labels_train[np.where(labels_train==3)] = 0
	labels_train[np.where(labels_train==8)] = 1
	
	labels_test[np.where(labels_test==3)]   = 0
	labels_test[np.where(labels_test==8)]   = 1

	# save to read in via c++
	np.savetxt("original/images_train.out", images_train, fmt='%f')
	np.savetxt("original/labels_train.out", labels_train, fmt='%i')
	np.savetxt("original/images_test.out",  images_test,  fmt='%f')
	np.savetxt("original/labels_test.out",  labels_test,  fmt='%i')
	# save reduced data
	np.savetxt("original/images_train_reduced.out", reduced_train, fmt='%f')
	np.savetxt("original/images_test_reduced.out",  reduced_test,  fmt='%f')
	
	# only save 50 instances for debugging purposes
	# save to read in via c++
	#np.savetxt("original/images_train.out", images_train[0:50], fmt='%f')
	#np.savetxt("original/labels_train.out", labels_train[0:50], fmt='%i')
	#np.savetxt("original/images_test.out",   images_test[0:50],  fmt='%f')
	#np.savetxt("original/labels_test.out",   labels_test[0:50],  fmt='%i')
	## save reduced data
	#np.savetxt("original/images_train_reduced.out", reduced_train[0:50], fmt='%f')
	#np.savetxt("original/images_test_reduced.out",   reduced_test[0:50],  fmt='%f')
