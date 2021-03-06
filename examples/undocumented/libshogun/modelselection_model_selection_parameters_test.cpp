/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Heiko Strathmann, Sergey Lisitsyn, Wu Lin
 */

#include <shogun/base/init.h>
#include <shogun/modelselection/ModelSelectionParameters.h>
#include <shogun/modelselection/ParameterCombination.h>
#include <shogun/kernel/GaussianKernel.h>
#include <shogun/kernel/PowerKernel.h>
#include <shogun/distance/MinkowskiMetric.h>
#include <shogun/kernel/string/DistantSegmentsKernel.h>
#include <shogun/mathematics/Math.h>

using namespace shogun;

void print_message(FILE* target, const char* str)
{
	fprintf(target, "%s", str);
}

CModelSelectionParameters* build_complex_example_tree()
{
	CModelSelectionParameters* root=new CModelSelectionParameters();

	CModelSelectionParameters* c=new CModelSelectionParameters("C");
	root->append_child(c);
	c->build_values(1.0, 1.0, R_EXP);

	CPowerKernel* power_kernel=new CPowerKernel();

	/* print all parameter available for modelselection
	 * Dont worry if yours is not included, simply write to the mailing list */
	power_kernel->print_modsel_params();

	CModelSelectionParameters* param_power_kernel=
			new CModelSelectionParameters("kernel", power_kernel);

	root->append_child(param_power_kernel);

	CModelSelectionParameters* param_power_kernel_degree=
			new CModelSelectionParameters("degree");
	param_power_kernel_degree->build_values(1.0, 1.0, R_EXP);
	param_power_kernel->append_child(param_power_kernel_degree);

	CMinkowskiMetric* m_metric=new CMinkowskiMetric(10);
	CModelSelectionParameters* param_power_kernel_metric1=
			new CModelSelectionParameters("distance", m_metric);

	param_power_kernel->append_child(param_power_kernel_metric1);

	CModelSelectionParameters* param_power_kernel_metric1_k=
			new CModelSelectionParameters("k");
	param_power_kernel_metric1_k->build_values(1.0, 12.0, R_LINEAR);
	param_power_kernel_metric1->append_child(param_power_kernel_metric1_k);

	CGaussianKernel* gaussian_kernel=new CGaussianKernel();

	/* print all parameter available for modelselection
	 * Dont worry if yours is not included, simply write to the mailing list */
	gaussian_kernel->print_modsel_params();

	CModelSelectionParameters* param_gaussian_kernel=
			new CModelSelectionParameters("kernel", gaussian_kernel);

	root->append_child(param_gaussian_kernel);

	CModelSelectionParameters* param_gaussian_kernel_width=
			new CModelSelectionParameters("log_width");
	param_gaussian_kernel_width->build_values(
	    0.0, 0.5 * std::log(2.0), R_LINEAR);
	param_gaussian_kernel->append_child(param_gaussian_kernel_width);

	CDistantSegmentsKernel* ds_kernel=new CDistantSegmentsKernel();

	/* print all parameter available for modelselection
	 * Dont worry if yours is not included, simply write to the mailing list */
	ds_kernel->print_modsel_params();

	CModelSelectionParameters* param_ds_kernel=new CModelSelectionParameters("kernel",
			ds_kernel);

	root->append_child(param_ds_kernel);

	CModelSelectionParameters* param_ds_kernel_delta=
			new CModelSelectionParameters("delta");
	param_ds_kernel_delta->build_values(1.0, 2.0, R_EXP);
	param_ds_kernel->append_child(param_ds_kernel_delta);

	CModelSelectionParameters* param_ds_kernel_theta=
			new CModelSelectionParameters("theta");
	param_ds_kernel_theta->build_values(1.0, 2.0, R_EXP);
	param_ds_kernel->append_child(param_ds_kernel_theta);

	return root;
}

CModelSelectionParameters* build_sgobject_no_childs_tree()
{
	CPowerKernel* power_kernel=new CPowerKernel();
	CModelSelectionParameters* param_power_kernel=
			new CModelSelectionParameters("kernel", power_kernel);

	return param_power_kernel;
}

CModelSelectionParameters* build_leaf_node_tree()
{
	CModelSelectionParameters* c_1=new CModelSelectionParameters("C1");
	c_1->build_values(1.0, 1.0, R_EXP);

	return c_1;
}

CModelSelectionParameters* build_root_no_childs_tree()
{
	return new CModelSelectionParameters();
}

CModelSelectionParameters* build_root_value_childs_tree()
{
	CModelSelectionParameters* root=new CModelSelectionParameters();

	CModelSelectionParameters* c_1=new CModelSelectionParameters("C1");
	root->append_child(c_1);
	c_1->build_values(1.0, 1.0, R_EXP);

	CModelSelectionParameters* c_2=new CModelSelectionParameters("C2");
	root->append_child(c_2);
	c_2->build_values(1.0, 1.0, R_EXP);

	return root;
}

CModelSelectionParameters* build_root_sg_object_child_tree()
{
	CModelSelectionParameters* root=new CModelSelectionParameters();

	CPowerKernel* power_kernel=new CPowerKernel();

	/* print all parameter available for modelselection
	 * Dont worry if yours is not included, simply write to the mailing list */
	power_kernel->print_modsel_params();

	CModelSelectionParameters* param_power_kernel=
			new CModelSelectionParameters("kernel", power_kernel);

	root->append_child(param_power_kernel);

	return root;
}

CModelSelectionParameters* build_root_sg_object_child_value_child_tree()
{
	CModelSelectionParameters* root=new CModelSelectionParameters();

	CPowerKernel* power_kernel=new CPowerKernel();

	/* print all parameter available for modelselection
	 * Dont worry if yours is not included, simply write to the mailing list */
	power_kernel->print_modsel_params();

	CModelSelectionParameters* param_power_kernel=
			new CModelSelectionParameters("kernel", power_kernel);

	CModelSelectionParameters* c=new CModelSelectionParameters("C");
	root->append_child(c);
	c->build_values(1.0, 1.0, R_EXP);

	root->append_child(param_power_kernel);

	return root;
}

void test_get_combinations(CModelSelectionParameters* tree)
{
	tree->print_tree();

	/* build combinations of parameter trees */
	CDynamicObjectArray* combinations=tree->get_combinations();

	/* print and directly delete them all */
	SG_SPRINT("----------------------------------\n");
	for (index_t i=0; i<combinations->get_num_elements(); ++i)
	{
		CParameterCombination* combination=(CParameterCombination*)
				combinations->get_element(i);
		combination->print_tree();
		SG_UNREF(combination);
	}

	SG_UNREF(combinations);
}

int main(int argc, char **argv)
{
	init_shogun(&print_message, &print_message, &print_message);

	CModelSelectionParameters* tree;

	tree=build_root_no_childs_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_leaf_node_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_sgobject_no_childs_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_root_value_childs_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_root_sg_object_child_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_root_sg_object_child_value_child_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	tree=build_complex_example_tree();
	SG_REF(tree);
	test_get_combinations(tree);
	SG_UNREF(tree);

	exit_shogun();

	return 0;
}

