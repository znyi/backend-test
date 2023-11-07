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
    const cppCommandString = CppAddon.getSdoCommand(JSON.stringify(req.body.variables.a))
    const buf = JSON.parse(cppCommandString)

    const obj = {
        variables : {
            a : req.body.variables.a
        },
        buffer : buf
    }
    res.json(obj)
})
app.post('/sine/step/2', (req, res)=>{
    console.log('step2')
    const readBufArr = Array.from(req.body.buffer)
    const cppOutputString = CppAddon.getSdoOutput(JSON.stringify(readBufArr))
    const aPrime = JSON.parse(cppOutputString)

    const cppCommandString = CppAddon.getSdoCommand(JSON.stringify(req.body.variables.b))
    const writeBuf = JSON.parse(cppCommandString)

    const obj = {
        variables : {
            a : req.body.variables.a,
            aPrime : aPrime,
            b : req.body.variables.b,
        },
        buffer : writeBuf
    }
    res.json(obj)
})
app.post('/sine/step/3', (req, res)=>{
    console.log('step3')
    const readBufArr = Array.from(req.body.buffer)
    const cppOutputString = CppAddon.getSdoOutput(JSON.stringify(readBufArr))
    const bPrime = JSON.parse(cppOutputString)

    const cppCommandString = CppAddon.getSdoCommand(JSON.stringify(req.body.variables.c))
    const writeBuf = JSON.parse(cppCommandString)

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
    res.json(obj)
})
app.post('/sine/step/4', (req, res)=>{
    console.log('step4')
    const readBufArr = Array.from(req.body.buffer)
    const cppOutputString = CppAddon.getSdoOutput(JSON.stringify(readBufArr))
    const cPrime = JSON.parse(cppOutputString)

    const outputString = CppAddon.getPdoExecCommand()
    const writeBuf = JSON.parse(outputString)

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
    res.json(obj)
})
app.get('/sine/stop', (req, res)=> { //to stop sine pdo
    console.log('sine stop')
    const outputString = CppAddon.getPdoStopCommand()
    const pdo_input_buffer = JSON.parse(outputString)
    res.json(pdo_input_buffer)
})
app.post('/sine/output', (req, res)=> {
    console.log('sine output')
    const variables = req.body.variables
    const a = variables.a
    const b = variables.b
    const c = variables.c

    const pdo_value_chunk = req.body.chunk //an array containing pdo raw values
    const pdo_output_chunk = pdo_value_chunk.map((elem)=>{
        const outputString = CppAddon.getPdoOutput(JSON.stringify(elem))
        const {data1,data2} = JSON.parse(outputString) //take the positive value as degree t
        const t = data1* (Math.PI/180)
        const coord = {
            data1: data1,
            t: t, 
            y: a*Math.sin(b*t + c)}
        return coord
    })
    res.json(pdo_output_chunk)
})
app.listen(PORT, ()=>{
    console.log(`app listening at port ${PORT}.`)
})
