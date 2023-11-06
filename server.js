import express from 'express';
import bodyParser from 'body-parser';
import cors from 'cors'
import { createRequire } from 'node:module'

//set up cpp addon
const require = createRequire(import.meta.url)
const CppAddon = require('./build/Release/CppAddon.node')

//set up web back end
const app = express()
const PORT = 5000

const corsOptions = {
    origin: 'http://localhost:3000', //'https://210.181.65.166:3100', //
    optionsSuccessStatus: 200 // some legacy browsers (IE11, various SmartTVs) choke on 204
}
app.use(cors(corsOptions))

app.use(bodyParser.json())
app.use(bodyParser.urlencoded({extended: true}))

app.get('/sdo/command', (req, res)=> {
    const sdo_a = req.query.sdo_a //not parseInt bcs i need to pass string to cpp function
    const outputString = CppAddon.getSdoCommand(sdo_a)
    const sdo_input_buffer = JSON.parse(outputString)
    res.json(sdo_input_buffer)
})
app.get('/sdo/output', (req, res)=> {
    const sdo_output_buffer = req.query.sdo_output_buffer //not JSON.parse bcs i need to pass string to cpp function
    const outputString = CppAddon.getSdoOutput(sdo_output_buffer)
    const sdo_a_increased = JSON.parse(outputString)
    res.json(sdo_a_increased)
})

app.get('/pdo/command/exec', (req, res)=> {
    const outputString = CppAddon.getPdoExecCommand()
    const pdo_input_buffer = JSON.parse(outputString)
    res.json(pdo_input_buffer)
})
app.get('/pdo/command/stop', (req, res)=> {
    const outputString = CppAddon.getPdoStopCommand()
    const pdo_input_buffer = JSON.parse(outputString)
    res.json(pdo_input_buffer)
})
app.get('/pdo/output', (req, res)=> {
    const pdo_output_buffer = req.query.pdo_output_buffer //not JSON.parse bcs i need to pass string to cpp function
    const outputString = CppAddon.getPdoOutput(pdo_output_buffer)
    const pdo_data_object = JSON.parse(outputString)
    res.json(pdo_data_object)
})

app.post('/sine/step/1', (req, res)=>{
    console.log('step1')
    console.log(req.body)
    const buf = [2, 20, 1, 2, 9, 1, 1, req.body.variables.a]
    const obj = {
        variables : {
            a : req.body.variables.a
        },
        buffer : buf
    }
    console.log(obj)
    res.json(obj)
})
app.post('/sine/step/2', (req, res)=>{
    console.log('step2')
    console.log(req.body)
    const readBufArr = Array.from(req.body.buffer)
    const aPrime = readBufArr[readBufArr.length-1]
    const writeBuf = [2, 20, 1, 2, 9, 1, 1, req.body.variables.b]
    const obj = {
        variables : {
            a : req.body.variables.a,
            aPrime : aPrime,
            b : req.body.variables.b,
        },
        buffer : writeBuf
    }
    console.log(obj)
    res.json(obj)
})
app.post('/sine/step/3', (req, res)=>{
    console.log('step3')
    console.log(req.body)
    const readBufArr = Array.from(req.body.buffer)
    const bPrime = readBufArr[readBufArr.length-1]
    const writeBuf = [2, 20, 1, 2, 9, 1, 1, req.body.variables.c]
    const obj = {
        variables : {
            a : req.body.variables.a,
            aPrime : req.body.variables.aPrime,
            b : req.body.variables.b,
            bPrime : bPrime,
            c : req.body.variables.c
        },
        buffer : writeBuf
    }
    console.log(obj)
    res.json(obj)
})
app.post('/sine/step/4', (req, res)=>{
    console.log('step4')
    console.log(req.body)
    const readBufArr = Array.from(req.body.buffer)
    const cPrime = readBufArr[readBufArr.length-1]
    const writeBuf = [
        2, 20, 4, 
        2, 1, 1, 1, 1, 
        2, 3, 1, 1, 1, 
        2, 4, 1, 2, 2, 0, 2, 1, 
        2, 1, 1, 1, 2
    ]
    const obj = {
        variables : {
            a : req.body.variables.a,
            aPrime : req.body.variables.aPrime,
            b : req.body.variables.b,
            bPrime : req.body.variables.bPrime,
            c : req.body.variables.c,
            cPrime : cPrime
        },
        buffer : writeBuf
    }
    console.log(obj)
    res.json(obj)
})

app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})
