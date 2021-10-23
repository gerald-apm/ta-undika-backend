const Joi = require('joi');

const SensorPayloadSchema = Joi.object({
    uuid: Joi.string().required(),
    sensors: Joi.array().items(Joi.object({
        type: Joi.string(),
        value: Joi.number().required(),
        valueUnit: Joi.string().required(),
        displayUnit: Joi.string().required(),
        minValue: Joi.number().required(),
        maxValue: Joi.number().min(Joi.ref('minValue')).required(),
    })).required(),
});

module.exports = {SensorPayloadSchema};
