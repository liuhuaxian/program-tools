#include "license.h"
#include "prod_config.h"
#include "lic_error.h"
#include "oplicense.h"

// enum the error str
const char * const err_str[] = { 
    "It is a formal license",
    "It is a test license (not expired)",
    "It is a test license but already expired",
    "Invalid serial number",
    "No license file",
    "It is not for this product",
    "Unknown errors",
    "no have alloc memory",
};

static int check_license(struct license_info *license_in, struct product_info *prod)
{
    assert (prod != NULL);

    if(prod->name != license_in->product_name) // wrong product name 
        return LICENSE_ERR_PRODUCT;

    if ((prod->version != license_in->version) || ((int)prod->release != license_in->release))
        return LICENSE_ERR_PRODUCT; // wrong product veriosn

    return 0;
}

void get_license_state(struct license_info *license_in, struct license_state *license_out) 
{
    struct stat statbuf;
    struct product_info info;
    int rc;
    time_t now;
    time_t gentime;

    //todo
    if (license_in == NULL || license_out == NULL) {
        license_out->state = STATE_LICENSE_NO_MEMORY;
        license_out->lefttime = 0;
        license_out->err = err_str[license_out->state];
        return;
    }

    if (stat(license_in->license_path, &statbuf) != 0) {
        if (errno == ENOENT) { //file not exist
            license_out->state = STATE_LICENSE_NONE;
        } else { //other errors.
            license_out->state = STATE_LICENSE_UNKNOWN;
        }
        license_out->lefttime = 0;
        license_out->err = err_str[license_out->state];
        return;
    }

    // decode license and get product information.
    rc = load_license(license_in->license_path, &info);

    if (rc == 0) { // load license success.
        gentime = test_mktime(&info);   // get the generated time of license.
        time(&now); // the current time in seconds.

        rc = lic_is_state(&info);
        switch (rc) {
            case LICENSE_FORMAL:
                if (check_license(license_in, &info) == LICENSE_ERR_PRODUCT) { //error product.
                    license_out->state = STATE_LICENSE_ERR_PRODUCT;
                    license_out->lefttime = 0;
                } else { // ok.
                    license_out->state = STATE_LICENSE_FORMAL;
                    license_out->lefttime = -1; 
                }
                break;
            case LICENSE_TEST_EXPIRED: // expired
                license_out->state = STATE_LICENSE_EXPIRED;
                license_out->lefttime = (unsigned int)(now + license_in->period_days*24*60*60 - gentime);//expired seconds
                break;
            case LICENSE_TEST_NOEXPIRED: // not expired
                if (check_license(license_in, &info) == LICENSE_ERR_PRODUCT) { //error product.
                    license_out->state =  STATE_LICENSE_ERR_PRODUCT;
                    license_out->lefttime = 0;
                } else {
                    license_out->state = STATE_LICENSE_TRIAL;
                    license_out->lefttime = (unsigned int)(gentime + license_in->period_days*24*60*60 - now);//left seconds
                }
                break;
            default: // other errors
                license_out->state = STATE_LICENSE_UNKNOWN;
                license_out->lefttime = 0;
                break;
        }        

    } else if (rc == LICENSE_ERR_EVAL) { // invalid serial number.
        license_out->state = STATE_LICENSE_INVALID;
        license_out->lefttime = 0;

    } else { // other errors
        license_out->state = STATE_LICENSE_UNKNOWN;
        license_out->lefttime = 0;
    }
    license_out->err = err_str[license_out->state];
}

int gen_license(const char* serial, const char* license_path)//generate license file from serial number.
{
    int err;
    struct product_info info;

    if(get_product_info(serial, &info)) {
        return 1;
    }

    err = reglic(license_path, serial, &info); // do generate
    if((err != LICENSE_TEST_NOEXPIRED) && (err != LICENSE_FORMAL)) {
        return 1;
    }
    return 0;
}

